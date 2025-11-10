//
// Created by Matteo Ranzi on 04/11/25.
//

#ifndef PIPEX_PIPELINE_H
#define PIPEX_PIPELINE_H

#include "PipeX/debug/pipex_print_debug.h"
#include "extended_cpp_standard/memory.h"


#include "nodes/Node.h"

#include <vector>
#include <memory>

namespace PipeX {
    template<typename T>
    class Pipeline {
    public:

        Pipeline() : name("NO_NAME") {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor()\n", name.c_str(), this);
        }
        explicit Pipeline(std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(std::string)\n", name.c_str(), this);
        }
        ~Pipeline() {
            PIPEX_PRINT_DEBUG_WARN("[Pipeline] \"%s\" {%p}.Destructor()\n", name.c_str(), this);
        }

        // Move constructors
        Pipeline (Pipeline&& _pipeline) noexcept{
            PIPEX_PRINT_DEBUG_WARN("[Pipeline] \"%s\" {%p}.Constructor(&&)\n", _pipeline.name.c_str(), this);
            *this = std::move(_pipeline);
        }

        Pipeline& operator=(Pipeline&& _pipeline) noexcept {
            PIPEX_PRINT_DEBUG_WARN("[Pipeline] \"%s\" {%p}.Operator=(&&)\n", _pipeline.name.c_str(), this);
            if (this != &_pipeline) {
                this->nodes = std::move(_pipeline.nodes);
                this->name = std::move(_pipeline.name);
            }
            return *this;
        }

        // Delete copy constructors
        // TODO : check how to properly implement factory pattern
        Pipeline& operator=(Pipeline const& _pipeline) {
            PIPEX_PRINT_DEBUG_WARN("[Pipeline] \"%s\" {%p}.Operator=(&)\n", _pipeline.name.c_str(), this);
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
        Pipeline(const Pipeline& _pipeline) {
            PIPEX_PRINT_DEBUG_WARN("[Pipeline] \"%s\" {%p}.Constructor(&)\n", _pipeline.name.c_str(), this);
            *this = _pipeline;
        }


        template<typename NodeT, typename... Args>
        Pipeline& addNode(Args&&... args) & {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.addNode()&\n", name.c_str(), this);
            nodes.push_back(make_unique<NodeT>(std::forward<Args>(args)...));

            return *this;
        }

        template<typename NodeT, typename... Args>
        Pipeline&& addNode(Args&&... args) && {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.addNode()&&\n", name.c_str(), this);
            nodes.push_back(make_unique<NodeT>(std::forward<Args>(args)...));

            return std::move(*this);
        }

        std::vector<T> run (std::vector<T> data) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.run() -> %zu nodes\n", name.c_str(), this, nodes.size());

            for (const auto& node : nodes) {
                data = node->process(std::move(data));
            }

            return data;
        }

        std::string getName() const { return name; }

    private:
        std::string name;
        std::vector<std::unique_ptr<Node<T, T>>> nodes;
    };
} // PipeX

#endif //PIPEX_PIPELINE_H