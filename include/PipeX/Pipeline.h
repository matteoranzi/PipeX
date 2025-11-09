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

#include "nodes/Filter.h"

namespace PipeX {
    template<typename T>
    class Pipeline {
    public:
        Pipeline() = default;
        ~Pipeline() = default;

        template<typename NodeT, typename... Args>
        Pipeline &addNode(Args&&... args) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] Adding node to pipeline\n");
            // TODO Implement the addNode logic

            return *this;
        }

        std::vector<T> run (std::vector<T> data) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] Running pipeline with %zu nodes\n", nodes.size());
            //TODO Implement the run logic
        }

    private:
        std::unique_ptr<std::vector<std::unique_ptr<Node<T, T>>>> nodes;
    };
} // PipeX

#endif //PIPEX_PIPELINE_H