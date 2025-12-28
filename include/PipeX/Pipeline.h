//
// Created by Matteo Ranzi on 19/12/25.
//

#ifndef PIPEX_PIPELINE_H
#define PIPEX_PIPELINE_H

#include <assert.h>

#include "PipeX/debug/pipex_print_debug.h"
#include "my_extended_cpp_standard/my_memory.h"

#include <string>
#include <vector>
#include <list>
#include <set>

#include "nodes/primitives/INode.h"
#include "data/IData.h"
#include "errors/InvalidPipelineException.h"
#include "errors/TypeMismatchExpection.h"
#include "nodes/primitives/Sink.h"
#include "nodes/primitives/Source.h"

//TODO add static_assert to check that InputT and OutputT are supported by the nodes added to the pipeline
// e.g. if a Transformer<InputT, IntermediateT> is added, InputT must match the Pipeline InputT
// and IntermediateT must match the InputT of the next node in the pipeline or the Pipeline OutputT if it's the last node

//TODO add SFINAE to restrict NodeT in addNode to only types derived from INode

//TODO add method to get the list of nodes in the pipeline (e.g. for visualization or debugging purposes)

//TODO a std::set of nodes names to avoid duplicates, throw exception if a node with the same name is added (if deletion by name is implemented)

namespace PipeX {

    /**
     * @class Pipeline
     * @brief A dynamic, type-erased pipeline that transforms a sequence of InputT into OutputT.
     *
     * The pipeline stores a list of nodes derived from INode which process data represented
     * by IData wrappers. Each node consumes and produces vectors of unique_ptr<IData>.
     *
     */

    class Pipeline {
    public:

        /**
         * @brief Default constructor.
         *
         * Sets the pipeline name to "NO_NAME" and logs construction.
         */
        Pipeline() : name([this]()->std::string {
                                        std::ostringstream oss;
                                        oss << this;
                                        return oss.str();
                                    }()
                                ) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor()\n", name.c_str(), this);
        }

        /**
         * @brief Construct a pipeline with a custom name.
         *
         * @param _name Pipeline name (moved into internal storage).
         */
        explicit Pipeline(std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(std::string)\n", name.c_str(), this);
        }

        /**
         * @brief Destructor.
         *
         * Logs destruction. Owned nodes are destroyed automatically.
         */
        ~Pipeline() {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Destructor()\n", name.c_str(), this);
        }

        /**
         * @brief Copy assignment operator.
         *
         * Performs deep copy of the nodes by calling clone() on each node in the source pipeline.
         * The copied pipeline receives the source name with a "_copy" suffix.
         *
         * @param _pipeline Source pipeline to copy from.
         * @return Reference to this pipeline.
         */
        Pipeline& operator=(Pipeline const& _pipeline) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Operator=(&)\n", _pipeline.name.c_str(), this);
            if (this != &_pipeline) {
                name = _pipeline.name + "_copy";
                nodes.clear();
                for (const auto& node : _pipeline.nodes) {
                    nodes.push_back(node->clone());
                }
                hasSourceNode = _pipeline.hasSourceNode;
                hasSinkNode = _pipeline.hasSinkNode;
            }

            return *this;
        }

        /**
         * @brief Copy constructor.
         *
         * Delegates to copy assignment operator to perform deep copy.
         *
         * @param _pipeline Source pipeline to copy from.
         */
        Pipeline(const Pipeline& _pipeline) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(&)\n", _pipeline.name.c_str(), this);
            *this = _pipeline;
        }


        /**
         * @brief Move constructor.
         *
         * Transfers ownership of nodes and name from the source pipeline.
         *
         * @param _pipeline Pipeline to move from.
         */
        Pipeline(Pipeline&& _pipeline) noexcept {
            *this = std::move(_pipeline);
        }

        /**
         * @brief Move assignment operator.
         *
         * Transfers ownership of the internal node list and the name.
         *
         * @param _pipeline Pipeline to move from.
         * @return Reference to this pipeline.
         */
        Pipeline& operator=(Pipeline&& _pipeline) noexcept {
            if (this != &_pipeline) {
                this->nodes = std::move(_pipeline.nodes);
                this->name = std::move(_pipeline.name);

                this->hasSourceNode = _pipeline.hasSourceNode;
                this->hasSinkNode = _pipeline.hasSinkNode;
                _pipeline.hasSourceNode = false;
                _pipeline.hasSinkNode = false;
            }

            return *this;
        }

        /**
         * @brief Add a node to the pipeline (lvalue ref qualified).
         *
         * Constructs a node of type NodeT in-place using the provided arguments and appends it
         * to the pipeline's node list.
         *
         * @tparam NodeT Concrete node type deriving from INode.
         * @tparam Args Variadic constructor argument types for NodeT.
         * @param args Constructor arguments forwarded to NodeT.
         * @return Reference to this pipeline (allows chaining).
         *
         * @note The debug log references member `node->name` but no local `node` variable is present
         *       in this scope; this is left unchanged from the original implementation.
         */
        template<typename NodeT, typename... Args>
        Pipeline& addNode(Args&&... args) & {
            auto newNode = checkPipelineIntegrity<NodeT>(std::forward<Args>(args)...);

            if (!newNode) {
                throw InvalidPipelineException(this->name, "Failed to create node of type " + std::string(typeid(NodeT).name()));
            }

            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.addNode(\"%s\")&\n", name.c_str(), this, newNode->name.c_str());
            nodes.push_back(std::move(newNode));
            return *this;
        }

        /**
         * @brief Add a node to the pipeline (rvalue ref qualified).
         *
         * Same as the lvalue overload but returns a rvalue reference to support temporaries.
         *
         * @tparam NodeT Concrete node type deriving from INode.
         * @tparam Args Variadic constructor argument types for NodeT.
         * @param args Constructor arguments forwarded to NodeT.
         * @return Rvalue reference to this pipeline (allows chaining on temporaries).
         */
        template<typename NodeT, typename... Args>
        Pipeline&& addNode(Args&&... args) && {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.addNode()&& --> std::move to addNode()&\n", name.c_str(), this);
            return std::move(addNode<NodeT>(std::forward<Args>(args)...));
        }

        // If no node with the given name is found, the pipeline remains unchanged.
        Pipeline& removeNodeByName(const std::string& nodeName) & {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.removeNodeByName(\"%s\")&\n", name.c_str(), this, nodeName.c_str());

            for (auto it = nodes.begin(); it != nodes.end(); ++it) {
                if ((*it)->name == nodeName) {
                    if ((*it)->isSource()) {
                        hasSourceNode = false;
                    } else if ((*it)->isSink()) {
                        hasSinkNode = false;
                    }

                    nodes.erase(it);
                    nodesNameSet.erase(nodeName);
                    return *this;
                }
            }
            return *this;
        }

        /**
         * @brief Run the pipeline on a vector of input values.
         *
         * The input values of type InputT are first wrapped in Data<InputT> and stored as
         * IData pointers. Each node processes the vector, replacing it with the node's output.
         * After all nodes have processed the data, the IData elements are dynamic_cast back
         * to Data<OutputT> and their contained values are extracted into the returned vector.
         *
         *
         * @throws TypeMismatchException If any intermediate or final IData cannot be cast to the
         *         expected Data<OutputT> type.
         * @throws Any exceptions propagated by node processing are rethrown after logging.
         */
        void run() const {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.run(std::vector<InputT>) -> %zu nodes\n", name.c_str(), this, nodes.size());

            std::string details;
            if (!isValid(details)) {
                throw InvalidPipelineException(this->name, "Cannot run pipeline, invalid configuration:" + details);
            }

            std::vector<std::unique_ptr<IData>> data;

            // Process through nodes
            for (const auto& node : nodes) {
                try {
                    PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.run() -> processing node \"%s\"\n", name.c_str(), this, node->name.c_str());
                    data = node->process(std::move(data));
                } catch (TypeMismatchException &e) {
                    PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p}.run() -> TypeMismatchException exception in node \"%s\": %s\n", name.c_str(), this, node->name.c_str(), e.what());
                    // Rethrow the exception to propagate it up the call stack
                    throw;
                } catch (...) {
                    PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p}.run() -> unknown exception in node \"%s\"\n", name.c_str(), this, node->name.c_str());
                    throw;
                }
            }
        }

        /**
         * @brief Get the pipeline name.
         *
         * @return Pipeline name by value.
         */
        std::string getName() const { return name; }

        bool isValid(std::string& details) const {
            if (!hasSourceNode) {
                details = " missing Source node";
                return false;
            }
            if (!hasSinkNode) {
                details = " missing Sink node";
                return false;
            }
            return true;
        }

    private:
        /**
         * @brief Human-readable name for the pipeline used in debug logs.
         */
        std::string name;
        std::set<std::string> nodesNameSet;

        /**
         * @brief List of dynamically allocated nodes (ownership).
         *
         * Nodes are stored as unique_ptr<DynamicNode> allowing polymorphic behavior.
         */
        std::list<std::unique_ptr<INode>> nodes;

        bool hasSourceNode = false;
        bool hasSinkNode = false;

        /**
         * @brief Checks pipeline integrity rules before adding a node.
         *
         * This template method enforces the following constraints:
         * - NodeT must derive from INode (compile-time check via static_assert)
         * - Only one Source node is allowed, and it must be the first node
         * - Only one Sink node is allowed, and it must be the last node
         * - No nodes can be added after a Sink node
         *
         * @tparam NodeT The type of node being added to the pipeline.
         *
         * @throws InvalidPipelineException If a Source node already exists in the pipeline.
         * @throws InvalidPipelineException If attempting to add a Source node when other nodes exist.
         * @throws InvalidPipelineException If a Sink node already exists in the pipeline.
         * @throws InvalidPipelineException If attempting to add a non-Sink node after a Sink node.
         *
         * @note This method updates the hasSourceNode and hasSinkNode flags based on the node type.
         */
        template <typename NodeT, typename... Args>
        //currently derived classes of Sink and Source are not recognised as Sink and Source respectively
        std::unique_ptr<NodeT> checkPipelineIntegrity(Args&&... args) {
            static_assert(std::is_base_of<INode, NodeT>::value, "template parameter of Pipeline::addNode must derive from INode");
            auto newNode = make_unique<NodeT>(std::forward<Args>(args)...);
            const auto castedNode = dynamic_cast<INode*>(newNode.get());

            // if (is_specialization_of<Source, NodeT>::value) {
            if (castedNode->isSource()) {
                if (hasSourceNode) {
                    throw InvalidPipelineException(this->name, "Pipeline can have only one Source node");
                }
                if (!nodes.empty()) {
                    throw InvalidPipelineException(this->name, "Source node must be the first node in the pipeline");
                }

                hasSourceNode = true;

            // } else if (is_specialization_of<Sink, NodeT>::value) {
            } else if (castedNode->isSink()) {
                if (hasSinkNode) {
                    throw InvalidPipelineException(this->name, "Pipeline can have only one Sink node");
                }
                hasSinkNode = true;
            } else if (hasSinkNode) {
                throw InvalidPipelineException(this->name, "Sink node must be the last node in the pipeline");
            }

            if (!nodesNameSet.insert(newNode->name).second) {
                throw InvalidPipelineException(this->name, "Node with name \"" + newNode->name + "\" already exists in the pipeline");
            }
            return newNode;
        }
    };
}

#endif //PIPEX_PIPELINE_H