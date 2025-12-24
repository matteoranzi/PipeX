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

#include "nodes/DynamicNode.h"
#include "data/GenericData.h"
#include "data/Data.h"

// FIXME: check for memory leaks
namespace PipeX {
    template <typename InputT, typename OutputT>
    class DynamicPipeline {
    public:

        DynamicPipeline() : name("NO_NAME") {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.Constructor()\n", name.c_str(), this);
        }
        explicit DynamicPipeline(std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(std::string)\n", name.c_str(), this);
        }
        ~DynamicPipeline() {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.Destructor()\n", name.c_str(), this);
        }

        //Copy constructor
        DynamicPipeline& operator=(DynamicPipeline const& _pipeline) {
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

        DynamicPipeline(const DynamicPipeline& _pipeline) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.Constructor(&)\n", _pipeline.name.c_str(), this);
            *this = _pipeline;
        }


        // Move constructors
        DynamicPipeline(DynamicPipeline&& _pipeline) noexcept {
            *this = std::move(_pipeline);
        }

        DynamicPipeline& operator=(DynamicPipeline&& _pipeline) noexcept {
            if (this != &_pipeline) {
                this->nodes = std::move(_pipeline.nodes);
                this->name = std::move(_pipeline.name);
            }

            return *this;
        }

        template<typename NodeT, typename... Args>
        DynamicPipeline& addNode(Args&&... args) & {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.addNode(\"%s\")&\n", name.c_str(), this, node->name.c_str());
            nodes.push_back(make_unique<NodeT>(std::forward<Args>(args)...));

            return *this;
        }

        template<typename NodeT, typename... Args>
        DynamicPipeline&& addNode(Args&&... args) && {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.addNode(\"%s\")&&\n", name.c_str(), this, node->name.c_str());
            nodes.push_back(make_unique<NodeT>(std::forward<Args>(args)...));

            return std::move(*this);
        }

        //fixme pass by reference or by value? WHY?
        std::vector<OutputT> run(const std::vector<InputT>& input) const {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.run(std::vector<InputT>) -> %zu nodes\n", name.c_str(), this, nodes.size());

            // Convert input to GenericData
            std::vector<std::unique_ptr<GenericData>> data;
            data.reserve(input.size());
            for (auto& item : input) {
                data.push_back(std::unique_ptr<GenericData>(make_unique<Data<InputT>>(item)));
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



        std::string getName() const { return name; }

    private:
        std::string name;
        std::list<std::unique_ptr<DynamicNode>> nodes;
    };
}

#endif //PIPEX_DYNAMICPIPELINE_H