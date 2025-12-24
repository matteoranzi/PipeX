//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_DYNAMICTRANSFORMER_H
#define PIPEX_DYNAMICTRANSFORMER_H
#include <functional>

#include "DynamicNode.h"
#include "debug/print_debug.h"
#include "PipeX/dynamic/data/GenericData.h"
#include "PipeX/dynamic/data/Data.h"
#include "extended_cpp_standard/memory.h"

namespace PipeX {
    template <typename InputT, typename OutputT>
    class DynamicTransformer final: public DynamicNode {
    public:
        using Function = std::function<OutputT(const InputT& data)>;

        explicit DynamicTransformer(Function _function) : function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] {%p}.Constructor(Function)\n", this);
        }
        DynamicTransformer(std::string _name, Function _function) : DynamicNode(std::move(_name)), function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.Constructor(std::string, Function)\n", this->name.c_str(), this);
        }
        DynamicTransformer(const DynamicTransformer& other) : DynamicNode(other), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.CopyConstructor(DynamicTransformer)\n", this->name.c_str(), this);
        }
        DynamicTransformer(const DynamicTransformer&other, std::string _name) : DynamicNode(other, std::move(_name)), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.CopyConstructor(DynamicTransformer, std::string)\n", this->name.c_str(), this);
        }
        DynamicTransformer(DynamicTransformer&& other) noexcept : DynamicNode(other), function(std::move(other.function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.CopyConstructor(DynamicTransformer, std::string)\n", this->name.c_str(), this);
        }
        ~DynamicTransformer() override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.Destructor()\n", this->name.c_str(), this);
        }

        std::unique_ptr<DynamicNode> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<DynamicTransformer>(*this);
        }

        std::unique_ptr<DynamicNode> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.clone(std::string)\n", this->name.c_str(), this);
            return make_unique<DynamicTransformer>(*this, std::move(_name));
        }

    private:
        Function function;

        std::vector<std::unique_ptr<GenericData>> processImpl(std::vector<std::unique_ptr<GenericData>>&& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] {%p}.processImpl(std::vector<Data_*>&)\n", this);

            std::vector<std::unique_ptr<GenericData>> output;
            output.reserve(input.size());
            for (const auto& data : input) {
                auto castedData = dynamic_cast<const Data<InputT>*>(data.get());
                if (!castedData) {
                    throw std::bad_cast();
                }
                output.push_back(make_unique<Data<OutputT>>(function(castedData->value)));
            }
            return output;
        }
    };
}

#endif //PIPEX_DYNAMICTRANSFORMER_H