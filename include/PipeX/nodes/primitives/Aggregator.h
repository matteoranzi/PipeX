//
// Created by Matteo Ranzi on 22/12/25.
//

#ifndef PIPEX_AGGREGATOR_HPP
#define PIPEX_AGGREGATOR_HPP

#include <functional>
#include <vector>
#include <string>
#include <utility>
#include <memory>

#include "NodeCRTP.h"

namespace PipeX {

    /**
     * @class Aggregator
     * @brief A specialized INode that aggregates input data vector of one type to a single output data of another type.
     *
     * This class processes a vector of input data, applies a user-defined aggregation function,
     * and produces a vector containing a single output value.
     * It supports cloning and copying, and provides debug information during its lifecycle.
     * The Aggregator node inherits from NodeCRTP using the CRTP pattern, allowing compile-time
     * polymorphism while maintaining type safety.
     *
     * @tparam InputT The type of the input data.
     * @tparam OutputT The type of the output data.
     *
     *  @code
     *  // Create a transformer that calculates the sum of integers
     *  Aggregator<int, int> sumAggregator([](const std::vector<int>& dataVector) {
     *      return std::accumulate(dataVector.begin(), dataVector.end(), 0);
     *  });
     *  @endcode
     */
    template <typename InputT, typename OutputT, typename MetadataT = IMetadata>
    class Aggregator: public NodeCRTP<Aggregator<InputT, OutputT, MetadataT>, InputT, OutputT, MetadataT> {

        using Base = NodeCRTP<Aggregator, InputT, OutputT, MetadataT>;
        friend Base;

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
            this->logLifeCycle("MoveConstructor(Aggregator&&)");
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


        /**
         * @brief Process implementation that aggregates an input vector into a single output value.
         *
         * Applies the configured aggregator function to the given input vector and returns a
         * newly allocated vector containing exactly one aggregated value.
         *
         * @param input A \c std::unique_ptr to the input \c std::vector of \c InputT. The pointer
         *              may be a rvalue; this function reads from \c *input. If \c input is null,
         *              an empty output vector is returned.
         * @return std::unique_ptr<std::vector<OutputT>> A unique pointer to a vector containing
         *         a single aggregated \c OutputT value produced by \c aggregatorFunction.
         *
         * @note The aggregator function receives a const reference to the input vector and
         *       must not attempt to modify it.
         * @note This method logs its lifecycle entry via \c logLifeCycle.
         */
        std::unique_ptr<std::vector<OutputT>> processImpl(std::unique_ptr<std::vector<InputT>>&& input) const override {
            this->logLifeCycle("processImpl(std::unique_ptr<std::vector<InputT>>&&)");

            // Apply aggregation function
            auto output = extended_std::make_unique<std::vector<OutputT>>();
            output->push_back(std::move(aggregatorFunction(*input)));

            return output;
        }
    };
}

#endif //PIPEX_AGGREGATOR_HPP