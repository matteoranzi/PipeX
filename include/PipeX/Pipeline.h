//
// Created by Matteo Ranzi on 19/12/25.
//

#ifndef PIPEX_PIPELINE_H
#define PIPEX_PIPELINE_H

#include "PipeX/debug/pipex_print_debug.h"
#include "my_extended_cpp_standard/my_memory.h"

#include <string>
#include <list>
#include <set>
#include <sstream>

#include "nodes/primitives/INode.h"
#include "data/IData.h"
#include "errors/InvalidOperation.h"
#include "errors/InvalidPipelineException.h"
#include "errors/MetadataTypeMismatchException.h"
#include "errors/PipeX_IO_Exception.h"
#include "errors/TypeMismatchExpection.h"





namespace PipeX {

    /**
     * @class Pipeline
     * @brief A dynamic, type-erased pipeline that transforms data.
     *
     * The pipeline stores a list of nodes derived from INode which process data represented
     * by IData wrappers. Each node consumes and produces unique_ptr<IData>.
     *
     * @note In order to be valid, a pipeline must contain a single source node as first node and a single sink node as last node.
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
        Pipeline(const Pipeline& _pipeline) : name(_pipeline.name + "_copy"),
                                              nodesNameSet(_pipeline.nodesNameSet),
                                              hasSourceNode(_pipeline.hasSourceNode),
                                              hasSinkNode(_pipeline.hasSinkNode) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(&)\n", name.c_str(), this);
            for (const auto& node : _pipeline.nodes) {
                nodes.push_back(node->clone());
            }
        }


        /**
         * @brief Move constructor.
         *
         * Transfers ownership of nodes and name from the source pipeline.
         *
         * @param _pipeline Pipeline to move from.
         */
        Pipeline(Pipeline&& _pipeline) noexcept : name(_pipeline.name + "_copy"),
                                              nodesNameSet(std::move(_pipeline.nodesNameSet)),
                                              nodes(std::move(_pipeline.nodes)),
                                              hasSourceNode(_pipeline.hasSourceNode),
                                              hasSinkNode(_pipeline.hasSinkNode){
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(&)\n", name.c_str(), this);
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
            try {
                auto newNode = checkPipelineIntegrity<NodeT>(std::forward<Args>(args)...);

                if (!newNode) {
                    throw InvalidPipelineException(this->name, "Failed to create node of type " + std::string(typeid(NodeT).name()));
                }

                PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.addNode(\"%s\")&\n", name.c_str(), this, newNode->getName().c_str());
                nodes.push_back(std::move(newNode));
            } catch (InvalidOperation& e) {
                PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p}.addNode() -> InvalidOperation exception: %s\n", name.c_str(), this, e.what());
                throw;
            } catch (PipeXException& e) {
                PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p}.addNode() -> PipeXException exception: %s\n", name.c_str(), this, e.what());
                throw;
            } catch (std::exception& e) {
                PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p}.addNode() -> unknown exception: %s\n", name.c_str(), this, e.what());
                throw;
            }
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
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p} :: removeNodeByName(\"%s\") &\n", name.c_str(), this, nodeName.c_str());

            for (auto it = nodes.begin(); it != nodes.end(); ++it) {
                if ((*it)->getName() == nodeName) {
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

            std::unique_ptr<IData> data;
            std::cout << "Valid pipeline \"" << name << "\" starting execution with " << nodes.size() << " nodes." << std::endl;
            // Process through nodes
            for (const auto& node : nodes) {
                try {
                    PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p} :: run() -> processing node \"%s\"\n", name.c_str(), this, node->getName().c_str());

                    data = node->process(std::move(data));
                } catch (TypeMismatchException &e) {
                    std::string err = "TypeMismatchException in pipeline '" + name + "' at node '" + node->getName() + "': " + e.what();
                    // PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p} :: run() -> %s\n", name.c_str(), this, err.c_str());
                    throw PipeXException(err);
                } catch (MetadataTypeMismatchException &e) {
                    std::string err = "MetadataTypeMismatchException in pipeline '" + name + "' at node '" + node->getName() + "': " + e.what();
                    // PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p} :: run() -> %s\n", name.c_str(), this, err.c_str());
                    throw PipeXException(err);
                } catch (InvalidOperation &e) {
                    std::string err = "InvalidOperation in pipeline '" + name + "' at node '" + node->getName() + "': " + e.what();
                    // PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p} :: run() -> %s\n", name.c_str(), this, err.c_str());
                    throw PipeXException(err);
                } catch (PipeX_IO_Exception &e) {
                    std::string err = "PipeX_IO_Exception in pipeline '" + name + "' at node '" + node->getName() + "': " + e.what();
                    // PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p} :: run() -> %s\n", name.c_str(), this, err.c_str());
                    throw PipeX_IO_Exception(err);
                } catch (PipeXException &e) {
                    std::string err = "PipeXException in pipeline '" + name + "' at node '" + node->getName() + "': " + e.what();
                    // PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p} :: run() -> %s\n", name.c_str(), this, err.c_str());
                    throw PipeXException(err);
                } catch (std::exception &e) {
                    std::string err = "Unknown exception in pipeline '" + name + "' at node '" + node->getName() + "': " + e.what();
                    // PIPEX_PRINT_DEBUG_ERROR("[Pipeline] \"%s\" {%p} :: run() -> %s\n", name.c_str(), this, err.c_str());
                    throw PipeXException(err);
                }
            }

            std::cout << "***Pipeline \"" << name << "\" execution completed." << std::endl;
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

        std::unique_ptr<NodeT> checkPipelineIntegrity(Args&&... args) {
            static_assert(std::is_base_of<INode, NodeT>::value, "template parameter of Pipeline::addNode must derive from INode");
            auto newNode = extended_std::make_unique<NodeT>(std::forward<Args>(args)...);
            const auto castedNode = dynamic_cast<INode*>(newNode.get());

            if (castedNode->isSource()) {
                if (hasSourceNode) {
                    throw InvalidPipelineException(this->name, "[checkPipelineIntegrity] Pipeline can have only one Source node");
                }
                if (!nodes.empty()) {
                    throw InvalidPipelineException(this->name, "[checkPipelineIntegrity] Source node must be the first node in the pipeline");
                }

                hasSourceNode = true;

            } else if (castedNode->isSink()) {
                if (hasSinkNode) {
                    throw InvalidPipelineException(this->name, "[checkPipelineIntegrity] Pipeline can have only one Sink node");
                }
                hasSinkNode = true;
            } else if (hasSinkNode) {
                throw InvalidPipelineException(this->name, "[checkPipelineIntegrity] Sink node must be the last node in the pipeline");
            }

            if (!nodesNameSet.insert(newNode->getName()).second) {
                throw InvalidPipelineException(this->name, "[checkPipelineIntegrity] Node with name \"" + newNode->getName() + "\" already exists in the pipeline");
            }
            return newNode;
        }
    };
}

#endif //PIPEX_PIPELINE_H

