//
// Created by Matteo Ranzi on 05/11/25.
//

#ifndef PIPEX_AGGREGATOR_H
#define PIPEX_AGGREGATOR_H

#include "Node.h"
#include "PipeX/debug/pipex_print_debug.h"

#include <functional>
#include <vector>

namespace PipeX {
    template <typename InputT, typename OutputT>
    class Aggregator final : public Node<InputT, OutputT> {
    public:
        using Function = std::function<OutputT(const std::vector<InputT>&&)>;

        explicit Aggregator (Function _function) : function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] {%s}.Constructor(Function)\n", this->name.c_str());
        }
        Aggregator (std::string _name, Function _function) : Node<InputT, OutputT>(std::move(_name)), function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] {%s}.Constructor(std::string, Function)\n", this->name.c_str());
        }
        ~Aggregator() override {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] {%s}.Destructor()\n", this->name.c_str());
        }

    private:
        Function function;

        std::vector<OutputT> processImpl(const std::vector<InputT>&& input) const override{
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] {%s}.processImpl(&&)\n", this->name.c_str());
            return { function(std::move(input)) };
        }
    };
} // PipeX

#endif //PIPEX_AGGREGATOR_H