// File: include/PipeX/dynamic/DynamicPipeline.h
//
// Created by Matteo Ranzi on 19/12/25.
//

#ifndef PIPEX_DYNAMICPIPELINE_H
#define PIPEX_DYNAMICPIPELINE_H

#include "PipeX/debug/pipex_print_debug.h"
#include "extended_cpp_standard/memory.h"

#include <string>
#include <vector>
#include <list>

#include "nodes/INode.h"
#include "data/IData.h"
#include "data/Data.h"

namespace PipeX {

    /**
     * @brief A dynamic, type-erased pipeline that transforms a sequence of InputT into OutputT.
     *
     * The pipeline stores a list of nodes derived from DynamicNode which process data represented
     * by GenericData wrappers. Each node consumes and produces vectors of unique_ptr<GenericData>.
     *
     * @tparam InputT  Type of the pipeline input elements.
     * @tparam OutputT Type of the pipeline output elements.
     */
    template <typename InputT, typename OutputT>
    class Pipeline {
    public:

        /**
         * @brief Default constructor.
         *
         * Sets the pipeline name to "NO_NAME" and logs construction.
         */
        Pipeline() : name("NO_NAME") {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.Constructor()\n", name.c_str(), this);
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
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.Destructor()\n", name.c_str(), this);
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
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.Operator=(&)\n", _pipeline.name.c_str(), this);
            if (this != &_pipeline) {
                name = _pipeline.name + "_copy";
                nodes.clear();
                for (const auto& node : _pipeline.nodes) {
                    nodes.push_back(node->clone());
                }
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
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.Constructor(&)\n", _pipeline.name.c_str(), this);
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
            }

            return *this;
        }

        /**
         * @brief Add a node to the pipeline (lvalue ref qualified).
         *
         * Constructs a node of type NodeT in-place using the provided arguments and appends it
         * to the pipeline's node list.
         *
         * @tparam NodeT Concrete node type deriving from DynamicNode.
         * @tparam Args Variadic constructor argument types for NodeT.
         * @param args Constructor arguments forwarded to NodeT.
         * @return Reference to this pipeline (allows chaining).
         *
         * @note The debug log references member `node->name` but no local `node` variable is present
         *       in this scope; this is left unchanged from the original implementation.
         */
        template<typename NodeT, typename... Args>
        Pipeline& addNode(Args&&... args) & {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.addNode(\"%s\")&\n", name.c_str(), this, node->name.c_str());
            nodes.push_back(make_unique<NodeT>(std::forward<Args>(args)...));

            return *this;
        }

        /**
         * @brief Add a node to the pipeline (rvalue ref qualified).
         *
         * Same as the lvalue overload but returns an rvalue reference to support temporaries.
         *
         * @tparam NodeT Concrete node type deriving from DynamicNode.
         * @tparam Args Variadic constructor argument types for NodeT.
         * @param args Constructor arguments forwarded to NodeT.
         * @return Rvalue reference to this pipeline (allows chaining on temporaries).
         */
        template<typename NodeT, typename... Args>
        Pipeline&& addNode(Args&&... args) && {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.addNode(\"%s\")&&\n", name.c_str(), this, node->name.c_str());
            nodes.push_back(make_unique<NodeT>(std::forward<Args>(args)...));

            return std::move(*this);
        }

        /**
         * @brief Run the pipeline on a vector of input values.
         *
         * The input values of type InputT are first wrapped in Data<InputT> and stored as
         * GenericData pointers. Each node processes the vector, replacing it with the node's output.
         * After all nodes have processed the data, the GenericData elements are dynamic_cast back
         * to Data<OutputT> and their contained values are extracted into the returned vector.
         *
         * @param input Vector of InputT input values.
         * @return Vector of OutputT results.
         *
         * @throws std::bad_cast If any intermediate or final GenericData cannot be cast to the
         *         expected Data<OutputT> type.
         * @throws Any exceptions propagated by node processing are rethrown after logging.
         */
        std::vector<OutputT> run(const std::vector<InputT>& input) const {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.run(std::vector<InputT>) -> %zu nodes\n", name.c_str(), this, nodes.size());

            // Convert input to GenericData
            std::vector<std::unique_ptr<IData>> data;
            data.reserve(input.size());
            for (auto& item : input) {
                data.push_back(std::unique_ptr<IData>(make_unique<Data<InputT>>(item)));
            }

            // Process through nodes
            for (const auto& node : nodes) {
                try {
                    PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.run() -> processing node \"%s\"\n", name.c_str(), this, node->name.c_str());
                    data = node->process(std::move(data));
                } catch (std::bad_cast &e) {
                    PIPEX_PRINT_DEBUG_ERROR("[DynamicPipeline] \"%s\" {%p}.run() -> bad_cast exception in node \"%s\": %s\n", name.c_str(), this, node->name.c_str(), e.what());
                    // Rethrow the exception to propagate it up the call stack
                    throw e;
                } catch (... ) {
                    PIPEX_PRINT_DEBUG_ERROR("[DynamicPipeline] \"%s\" {%p}.run() -> unknown exception in node \"%s\"\n", name.c_str(), this, node->name.c_str());
                    throw;
                }
            }

            // Convert back to OutputT
            std::vector<OutputT> output;
            output.reserve(data.size());
            for (auto& genericData : data) {
                auto castedData = dynamic_cast<Data<OutputT>*>(genericData.get());
                if (!castedData) {
                    throw std::bad_cast();
                }
                output.push_back(castedData->value);
            }

            return output;
        }

        /**
         * @brief Get the pipeline name.
         *
         * @return Pipeline name by value.
         */
        std::string getName() const { return name; }

    private:
        /**
         * @brief Human-readable name for the pipeline used in debug logs.
         */
        std::string name;

        /**
         * @brief List of dynamically allocated nodes (ownership).
         *
         * Nodes are stored as unique_ptr<DynamicNode> allowing polymorphic behavior.
         */
        std::list<std::unique_ptr<INode>> nodes;
    };
}

#endif //PIPEX_DYNAMICPIPELINE_H