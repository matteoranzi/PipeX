//
// Created by Matteo Ranzi on 22/12/25.
//

#ifndef PIPEX_DYNAMICAGGREGATOR_HPP
#define PIPEX_DYNAMICAGGREGATOR_HPP

#include <functional>

#include "DynamicNode.h"
#include "debug/print_debug.h"
#include "PipeX/dynamic/data/GenericData.h"
#include "extended_cpp_standard/memory.h"
#include "PipeX/debug/pipex_print_debug.h"

namespace PipeX {
    class DynamicAggregator final: public DynamicNode {
    public:
        using Function = std::function<std::unique_ptr<GenericData>(const std::vector<std::unique_ptr<GenericData>>&&)>;

        explicit DynamicAggregator(Function _function) : function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] {%p}.Constructor(Function)\n", this);
        }
        DynamicAggregator(std::string _name, Function _function) : DynamicNode(std::move(_name)), function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.Constructor(std::string, Function)\n", this->name.c_str(), this);
        }
        DynamicAggregator(const DynamicAggregator& other) : DynamicNode(other), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.CopyConstructor(DynamicAggregator)\n", this->name.c_str(), this);
        }
        DynamicAggregator(const DynamicAggregator&other, std::string _name) : DynamicNode(other, std::move(_name)), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.CopyConstructor(DynamicAggregator, std::string)\n", this->name.c_str(), this);
        }
        DynamicAggregator(DynamicAggregator&& other) noexcept : DynamicNode(other), function(std::move(other.function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.CopyConstructor(DynamicAggregator, std::string)\n", this->name.c_str(), this);
        }

        ~DynamicAggregator() override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.Destructor()\n", this->name.c_str(), this);
        }

        std::unique_ptr<DynamicNode> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<DynamicAggregator>(*this);
        }

        std::unique_ptr<DynamicNode> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.clone(std::string)\n", this->name.c_str(), this);
            return make_unique<DynamicAggregator>(*this, std::move(_name));
        }

    private:
        Function function;

        std::vector<std::unique_ptr<GenericData>> processImpl(std::vector<std::unique_ptr<GenericData>>&& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] {%p}.processImpl(std::vector<Data_*>&)\n", this);
            std::vector<std::unique_ptr<GenericData>> result;
            result.push_back(function(std::move(input)));
            return result;
        }
    };
}

#endif //PIPEX_DYNAMICAGGREGATOR_HPP