//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_DYNAMICTRANSFORMER_H
#define PIPEX_DYNAMICTRANSFORMER_H
#include <functional>

#include "INode.h"
#include "PipeX/data/IData.h"
#include "PipeX/data/Data.h"
#include "extended_cpp_standard/memory.h"

namespace PipeX {
    template <typename InputT, typename OutputT>
    class Transformer final: public INode {
    public:
        using Function = std::function<OutputT(const InputT& data)>;

        explicit Transformer(Function _function) : function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] {%p}.Constructor(Function)\n", this);
        }
        Transformer(std::string _name, Function _function) : INode(std::move(_name)), function(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.Constructor(std::string, Function)\n", this->name.c_str(), this);
        }
        Transformer(const Transformer& other) : INode(other), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.CopyConstructor(DynamicTransformer)\n", this->name.c_str(), this);
        }
        Transformer(const Transformer&other, std::string _name) : INode(other, std::move(_name)), function(other.function) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.CopyConstructor(DynamicTransformer, std::string)\n", this->name.c_str(), this);
        }
        Transformer(Transformer&& other) noexcept : INode(other), function(std::move(other.function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.CopyConstructor(DynamicTransformer, std::string)\n", this->name.c_str(), this);
        }
        ~Transformer() override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.Destructor()\n", this->name.c_str(), this);
        }

        std::unique_ptr<INode> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<Transformer>(*this);
        }

        std::unique_ptr<INode> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.clone(std::string)\n", this->name.c_str(), this);
            return make_unique<Transformer>(*this, std::move(_name));
        }

    private:
        Function function;

        std::vector<std::unique_ptr<IData>> processImpl(const std::vector<std::unique_ptr<IData>>& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] {%p}.processImpl(std::vector<std::unique_ptr<GenericData>>&&)\n", this);

            std::vector<std::unique_ptr<IData>> output;
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