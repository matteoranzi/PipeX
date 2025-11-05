//
// Created by Matteo Ranzi on 04/11/25.
//

#pragma once

#include <vector>
#include <string>
#include <sstream>

#include <PipeX/debug/pipex_print_debug.h>


namespace PipeX {
    template <typename InputT, typename OutputT>
    class Node {
    public:
        const std::string name;
        Node () : name([this]() {
                            std::ostringstream oss;
                            oss << this;
                            return oss.str();
                        }()) {
            PIPEX_PRINT_DEBUG_INFO("[Node] {%s}.Constructor()\n", name.c_str());
        }
        explicit Node (std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[Node] {%s}.Constructor(std::string)\n", name.c_str());
        }
        virtual ~Node() = default;

        virtual std::vector<OutputT> process(const std::vector<InputT>& input) const = 0;
        virtual std::vector<OutputT> operator() (const std::vector<InputT>& input) const final {
            PIPEX_PRINT_DEBUG_INFO("[Node] {%s}.Operator()\n", name.c_str());
            return this->process(input);
        }
    };
}