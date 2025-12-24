//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_DYNAMICTRANSFORMER_H
#define PIPEX_DYNAMICTRANSFORMER_H
#include <functional>

#include "INode.h"
#include "NodeCRTP.h"
#include "PipeX/data/IData.h"
#include "PipeX/data/Data.h"
#include "extended_cpp_standard/memory.h"
#include "PipeX/errors/TypeMismatchExpection.h"

namespace PipeX {
    template <typename InputT, typename OutputT>
    class Transformer final: public NodeCRTP<Transformer<InputT, OutputT>, InputT, OutputT> {

        using Base = NodeCRTP<Transformer<InputT, OutputT>, InputT, OutputT>;
        friend Base;

    public:
        using Function = std::function<OutputT(const InputT& data)>;

        explicit Transformer(Function _function) : Base(), transformerFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(Function)");
        }
        Transformer(std::string _name, Function _function) : Base(std::move(_name)), transformerFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(std::string, Function)");
        }
        Transformer(const Transformer& other) : Base(other), transformerFunction(other.transformerFunction) {
            this->logLifeCycle("Constructor(const Transformer&)");
        }
        Transformer(const Transformer&other, std::string _name) : Base(other, std::move(_name)), transformerFunction(other.transformerFunction) {
            this->logLifeCycle("Constructor(const Transformer&, std::string)");
        }
        Transformer(Transformer&& other) noexcept : Base(other), transformerFunction(std::move(other.transformerFunction)) {
            this->logLifecycle("MoveConstructor(Transformer)");
        }
        ~Transformer() override {
             this->logLifeCycle("Destructor()");
        }

        std::unique_ptr<INode> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<Transformer>(*this);
        }

        std::unique_ptr<INode> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicTransformer] \"%s\" {%p}.clone(std::string)\n", this->name.c_str(), this);
            return make_unique<Transformer>(*this, std::move(_name));
        }

    protected:
        std::string typeName() const override {
            return "Transformer";
        }

    private:
        Function transformerFunction;

        std::vector<std::unique_ptr<IData>> processImpl(const std::vector<std::unique_ptr<IData>>& input) const override {
            this->logLifeCycle("processImpl(std::vector<std::unique_ptr<IData>>&&)");

            //Fixme: the solution with CRTP helper functions is cleaner but less efficient: a copy of the entire input is created (extracted) and stored

            // Extract input data using Base helper
            auto inputData = this->extractInputData(input);

            // Transform data
            std::vector<OutputT> outputData;
            outputData.reserve(inputData.size());
            for (const auto& data : inputData) {
                outputData.push_back(transformerFunction(data));
            }

            // Wrap output data using Base helper
            return this->wrapOutputData(std::move(outputData));
        }
    };
}

#endif //PIPEX_DYNAMICTRANSFORMER_H