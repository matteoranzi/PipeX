//
// Created by Matteo Ranzi on 04/11/25.
//

#pragma once

#include <PipeX/nodes/Node.h>

#include <functional>
#include <vector>

namespace PipeX {

    template <typename InputT, typename OutputT>
    class Transformer final : public Node<InputT, OutputT> {
    public:
        using Function = std::function<OutputT(const InputT&)>;

        explicit Transformer (Function _function) : function(std::move(_function)) {
            PRINT_DEBUG_INFO("[PipeX::Transformer] {%s}.Constructor(Function)\n", this->name.c_str());
        }
        Transformer (std::string _name, Function _function) : Node<InputT, OutputT>(std::move(_name)), function(std::move(_function)) {
            PRINT_DEBUG_INFO("[PipeX::Transformer] {%s}.Constructor(std::string, Function)\n", this->name.c_str());
        }
        ~Transformer() override =default;

        std::vector<OutputT> process(const std::vector<InputT>& input) const override {
            PRINT_DEBUG_INFO("[PipeX::Transformer] {%s}.process()\n", this->name.c_str());
            std::vector<OutputT> output;
            output.reserve(input.size());
            for (const auto& item : input) {
                output.push_back(function(item));
            }
            return output;
        }

    private:
        Function function;
    };
}
