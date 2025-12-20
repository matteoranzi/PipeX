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
#include "data/Data_.h"
#include "data/Data.h"
// FIXME: check for memory leaks
namespace PipeX {
    class DynamicPipeline {
    public:

        explicit DynamicPipeline(std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[Pipeline] \"%s\" {%p}.Constructor(std::string)\n", name.c_str(), this);
        }



    private:
        std::string name;
        // std::vector<std::unique_ptr<Node<Data_, Data_>>> nodes;
    };
}

#endif //PIPEX_DYNAMICPIPELINE_H