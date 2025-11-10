//
// Created by Matteo Ranzi on 04/11/25.
//

#ifndef PIPEX_TRANSFORMER_H
#define PIPEX_TRANSFORMER_H

#include "Node.h"
#include "PipeX/debug/pipex_print_debug.h"

#include <extended_cpp_standard/memory.h>

#include <functional>
#include <vector>

namespace PipeX {

    template <typename InputT, typename OutputT>
    class Transformer final : public Node<InputT, OutputT> {
    public:
        using Function = std::function<OutputT(const InputT&&)>;

        explicit Transformer (Function _function) : function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] {%s}.Constructor(Function)\n", this->name.c_str());
        }
        Transformer (std::string _name, Function _function) : Node<InputT, OutputT>(std::move(_name)), function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] {%s}.Constructor(std::string, Function)\n", this->name.c_str());
        }
        ~Transformer() override {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] {%s}.Destructor()\n", this->name.c_str());
        }

        std::unique_ptr<Node<int,int>> clone() const override {
            return make_unique<Transformer>(*this);
        }

    private:
        Function function;

        std::vector<OutputT> processImpl(const std::vector<InputT>&& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] {%s}.processImpl(&&)\n", this->name.c_str());
            std::vector<OutputT> output;
            output.reserve(input.size());
            for (const auto& item : input) {
                output.push_back(function(std::move(item)));
            }
            return output;
        }
    };
}

#endif

