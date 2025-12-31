//
// Created by Matteo Ranzi on 22/12/25.
//

#ifndef PIPEX_AGGREGATOR_HPP
#define PIPEX_AGGREGATOR_HPP

#include <functional>

#include "NodeCRTP.h"
#include "PipeX/data/IData.h"

namespace PipeX {

    /**
     * @class Aggregator
     * @brief A specialized INode that aggregates input data vector of one type to a single output data of another type.
     *
     * This class processes a vector of input data, applies a user-defined aggregation function,
     * and produces a single output value.
     * It supports cloning and copying, and provides debug information during its lifecycle.
     * The Aggregator node inherits from NodeCRTP using the CRTP pattern, allowing compile-time
     * polymorphism while maintaining type safety.
     *
     * @tparam InputT The type of the input data.
     * @tparam OutputT The type of the output data.
     *
     *  @code
     *  // Create a transformer that calculates the square of an integer
     *  Aggregator<int, int> sumAggregator([](const std::vector<int>& dataVector) {
     *      return std::accumulate(dataVector.begin(), dataVector.end(), 0);
     *  });
     *  @endcode
     */
    template <typename InputT, typename OutputT>
    class Aggregator: public NodeCRTP<Aggregator<InputT, OutputT>, InputT, OutputT> {

        using Base = NodeCRTP<Aggregator<InputT, OutputT>, InputT, OutputT>;

    public:
        
        using Function = std::function<OutputT(const std::vector<InputT>& dataVector)>;

        
        explicit Aggregator(Function _function) : Base(), aggregatorFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(Function)");
        }

        
        Aggregator(std::string _name, Function _function) : Base(std::move(_name)), aggregatorFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(std::string, Function)");
        }

        
        Aggregator(const Aggregator& other) : Base(other), aggregatorFunction(other.aggregatorFunction) {
            this->logLifeCycle("CopyConstructor(const Aggregator&)");
        }

        
        Aggregator(const Aggregator& other, std::string _name) : Base(other, std::move(_name)), aggregatorFunction(other.aggregatorFunction) {
            this->logLifeCycle("CopyConstructor(const Aggregator&, std::string)");
        }

        
        Aggregator(Aggregator&& other) noexcept : Base(other), aggregatorFunction(std::move(other.aggregatorFunction)) {
            this->logLifecycle("MoveConstructor(Aggregator&&)");
        }

        
        ~Aggregator() override {
            this->logLifeCycle("Destructor()");
        }

        virtual bool isSource() const final { return  false; }
        virtual bool isSink() const final { return  false; }

    protected:
        
        std::string typeName() const override {
            return "Aggregator";
        }

    private:
        Function aggregatorFunction;

        
        std::unique_ptr<IData> processImpl(const std::unique_ptr<IData>& input) const override {
            this->logLifeCycle("processImpl(std::vector<std::unique_ptr<IData>>&)");

            // Extract input data
            auto inputData = this->extractInputData(input);

            // Apply aggregation function
            std::vector<OutputT> outputData;
            outputData.push_back(std::move(aggregatorFunction(*inputData)));

            // Wrap output data back into IData
            return this->wrapOutputData(make_unique<std::vector<OutputT>>(std::move(outputData)));
        }
    };
}

#endif //PIPEX_AGGREGATOR_HPP