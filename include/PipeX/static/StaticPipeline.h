//
// Created by Matteo Ranzi on 04/11/25.
//

#ifndef PIPEX_PIPELINE_H
#define PIPEX_PIPELINE_H

#include "PipeX/debug/pipex_print_debug.h"
#include "extended_cpp_standard/memory.h"


#include "PipeX/static/nodes/StaticNode.h"

#include <vector>
#include <list>
#include <memory>

namespace PipeX {
    template<typename T>
    // TODO : check how to properly implement factory pattern
    class StaticPipeline {
    public:

        StaticPipeline() : name("NO_NAME") {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor()\n", name.c_str(), this);
        }
        explicit StaticPipeline(std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(std::string)\n", name.c_str(), this);
        }
        ~StaticPipeline() {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Destructor()\n", name.c_str(), this);
        }

        // Move constructors
        StaticPipeline (StaticPipeline&& _pipeline) noexcept{
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(&&)\n", _pipeline.name.c_str(), this);
            *this = std::move(_pipeline);
        }

        StaticPipeline& operator=(StaticPipeline&& _pipeline) noexcept {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Operator=(&&)\n", _pipeline.name.c_str(), this);
            if (this != &_pipeline) {
                this->nodes = std::move(_pipeline.nodes);
                this->name = std::move(_pipeline.name);
            }
            return *this;
        }

        // Copy constructors
        StaticPipeline& operator=(StaticPipeline const& _pipeline) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Operator=(&)\n", _pipeline.name.c_str(), this);
            if (this != &_pipeline) {
                name = _pipeline.name + "_copy";
                nodes.clear();
                for (const auto& node : _pipeline.nodes) {
                    nodes.push_back(node->clone());
                }
            }

            return *this;
        }
        StaticPipeline(const StaticPipeline& _pipeline) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(&)\n", _pipeline.name.c_str(), this);
            *this = _pipeline;
        }


        template<typename NodeT, typename... Args>
        StaticPipeline& addNode(Args&&... args) & {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.addNode(Args&&...)&\n", name.c_str(), this);
            nodes.push_back(make_unique<NodeT>(std::forward<Args>(args)...));

            return *this;
        }

        template<typename NodeT, typename... Args>
        StaticPipeline&& addNode(Args&&... args) && {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.addNode(Args&&...)&&\n", name.c_str(), this);
            nodes.push_back(make_unique<NodeT>(std::forward<Args>(args)...));

            return std::move(*this);
        }

        std::vector<T> run(std::vector<T> data) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.run() -> %zu nodes\n", name.c_str(), this, nodes.size());

            for (const auto& node : nodes) {
                data = node->process(std::move(data));
            }

            return data;
        }

        std::string getName() const { return name; }

    private:
        std::string name;
        // TODO std::vector or std::list ?
        std::list<std::unique_ptr<StaticNode<T, T>>> nodes;
    };
} // PipeX

#endif //PIPEX_PIPELINE_H