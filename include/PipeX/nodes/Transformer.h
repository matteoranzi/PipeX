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
            PIPEX_PRINT_DEBUG_INFO("[Transformer] \"%s\" {%p}.Constructor(Function)\n", this->name.c_str(), this);
        }
        Transformer (std::string _name, Function _function) : Node<InputT, OutputT>(std::move(_name)), function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] \"%s\" {%p}.Constructor(std::string, Function)\n", this->name.c_str(), this);
        }
        Transformer (const Transformer& other) : Node<InputT, OutputT>(other), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] \"%s\" {%p}.CopyConstructor()\n", this->name.c_str(), this);
        }
        Transformer (const Transformer& other, std::string _name) : Node<InputT, OutputT>(other, std::move(_name)), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] \"%s\" {%p}.CopyConstructor()\n", this->name.c_str(), this);
        }
        ~Transformer() override {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] \"%s\" {%p}.Destructor()\n", this->name.c_str(), this);
        }

        std::unique_ptr<Node<int,int>> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<Transformer>(*this);
        }
        std::unique_ptr<Node<int,int>> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<Transformer>(*this, std::move(_name));
        }

    private:
        Function function;

        std::vector<OutputT> processImpl(const std::vector<InputT>&& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[Transformer] \"%s\" {%p}.processImpl(&&)\n", this->name.c_str(), this);
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

