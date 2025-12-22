//
// Created by Matteo Ranzi on 19/12/25.
//

#ifndef PIPEX_DYNAMICPIPELINE_H
#define PIPEX_DYNAMICPIPELINE_H

#include "PipeX/debug/pipex_print_debug.h"
#include "extended_cpp_standard/memory.h"

#include <string>
#include <vector>

#include "nodes/DynamicNode.h"
#include "data/GenericData.h"
#include "data/Data.h"
// FIXME: check for memory leaks
namespace PipeX {
    class DynamicPipeline {
    public:

        DynamicPipeline() : name("NO_NAME") {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.Constructor()\n", name.c_str(), this);
        }
        explicit DynamicPipeline(std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(std::string)\n", name.c_str(), this);
        }
        ~DynamicPipeline() {
            PIPEX_PRINT_DEBUG_WARN("[DynamicPipeline] \"%s\" {%p}.Destructor()\n", name.c_str(), this);
        }

        //Copy constructor
        DynamicPipeline& operator=(DynamicPipeline const& _pipeline) {
            PIPEX_PRINT_DEBUG_WARN("[DynamicPipeline] \"%s\" {%p}.Operator=(&)\n", _pipeline.name.c_str(), this);
            if (this != &_pipeline) {
                name = _pipeline.name + "_copy";
                nodes.clear();
                nodes.reserve(_pipeline.nodes.size());
                for (const auto& node : _pipeline.nodes) {
                    nodes.push_back(node->clone());
                }
            }

            return *this;
        }

        DynamicPipeline(const DynamicPipeline& _pipeline) {
            PIPEX_PRINT_DEBUG_WARN("[DynamicPipeline] \"%s\" {%p}.Constructor(&)\n", _pipeline.name.c_str(), this);
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

        //FIXME: addNode passing parameter by reference or by value? WHY?
        //FIXME: shared_ptr vs unique_ptr? WHY? Is it clear inside the method a new copy will be created?
        DynamicPipeline& addNode(const std::shared_ptr<DynamicNode>& node) & {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.addNode(\"%s\")&\n", name.c_str(), this, node->name.c_str());
            nodes.push_back(std::unique_ptr<DynamicNode>(node->clone()));
            return *this;
        }

        DynamicPipeline&& addNode(const std::shared_ptr<DynamicNode>& node) && {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.addNode(\"%s\")&&\n", name.c_str(), this, node->name.c_str());
            return std::move(*this);
        }

        std::vector<std::unique_ptr<GenericData>> run(std::vector<std::unique_ptr<GenericData>> data) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicPipeline] \"%s\" {%p}.run(std::vector<std::unique_ptr<GenericData>>) -> %zu nodes\n", name.c_str(), this, nodes.size());
            for (const auto& node : nodes) {
                data = node->process(std::move(data));
            }

            return data;
        }

        std::string getName() const { return name; }

    private:
        std::string name;
        std::vector<std::unique_ptr<DynamicNode>> nodes;
    };
}

#endif //PIPEX_DYNAMICPIPELINE_H