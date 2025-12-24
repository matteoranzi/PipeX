//
// Created by Matteo Ranzi on 05/11/25.
//

#ifndef PIPEX_AGGREGATOR_H
#define PIPEX_AGGREGATOR_H

#include "StaticNode.h"
#include "PipeX/debug/pipex_print_debug.h"

#include "extended_cpp_standard/memory.h"

#include <functional>
#include <vector>

namespace PipeX {
    template <typename InputT, typename OutputT>
    class StaticAggregator final : public StaticNode<InputT, OutputT> {
    public:
        using Function = std::function<OutputT(const std::vector<InputT>&&)>;

        explicit StaticAggregator (Function _function) : function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] \"%s\" {%p}.Constructor(Function)\n", this->name.c_str(), this);
        }
        StaticAggregator (std::string _name, Function _function) : StaticNode<InputT, OutputT>(std::move(_name)), function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] \"%s\" {%p}.Constructor(std::string, Function)\n", this->name.c_str(), this);
        }
        StaticAggregator (const StaticAggregator& other) : StaticNode<InputT, OutputT>(other), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] \"%s\" {%p}.CopyConstructor()\n", this->name.c_str(), this);
        }
        StaticAggregator (const StaticAggregator& other, std::string _name) : StaticNode<InputT, OutputT>(other, std::move(_name)), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] \"%s\" {%p}.CopyConstructor()\n", this->name.c_str(), this);
        }
        ~StaticAggregator() override {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] \"%s\" {%p}.Destructor()\n", this->name.c_str(), this);
        }

        std::unique_ptr<StaticNode<InputT,OutputT>> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<StaticAggregator>(*this);
        }
        std::unique_ptr<StaticNode<InputT,OutputT>> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] \"%s\" {%p}.clone(std::string)\n", this->name.c_str(), this);
            return make_unique<StaticAggregator>(*this, std::move(_name));
        }

    private:
        Function function;

        std::vector<OutputT> processImpl(const std::vector<InputT>&& input) const override{
            PIPEX_PRINT_DEBUG_INFO("[Aggregator] \"%s\" {%p}.processImpl(&&)\n", this->name.c_str(), this);
            return { function(std::move(input)) };
        }
    };
} // PipeX

#endif //PIPEX_AGGREGATOR_H