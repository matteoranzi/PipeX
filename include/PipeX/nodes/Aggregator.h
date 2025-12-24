//
// Created by Matteo Ranzi on 22/12/25.
//

#ifndef PIPEX_DYNAMICAGGREGATOR_HPP
#define PIPEX_DYNAMICAGGREGATOR_HPP

#include <functional>

#include "INode.h"
#include "debug/print_debug.h"
#include "PipeX/data/IData.h"
#include "PipeX/data/Data.h"
#include "extended_cpp_standard/memory.h"
#include "PipeX/debug/pipex_print_debug.h"

namespace PipeX {

    /**
     * @brief A class representing a dynamic aggregator node in the PipeX framework.
     *
     * This class processes a vector of input data, applies a user-defined aggregation function,
     * and produces a single output value encapsulated in a `IData` object.
     *
     * @tparam InputT The type of the input data.
     * @tparam OutputT The type of the output data.
     */
    template <typename InputT, typename OutputT>
    class Aggregator final: public INode {
    public:
        /**
         * @brief Type alias for the aggregation function.
         *
         * The function takes a vector of `InputT` and returns a single `OutputT`.
         */
        using Function = std::function<OutputT(const std::vector<InputT>& dataVector)>;

        /**
         * @brief Constructs a `DynamicAggregator` with the specified aggregation function.
         *
         * @param _function The aggregation function to be used.
         */
        explicit Aggregator(Function _function) : aggregatorFunction(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] {%p}.Constructor(Function)\n", this);
        }

        /**
         * @brief Constructs a `DynamicAggregator` with a name and aggregation function.
         *
         * @param _name The name of the aggregator.
         * @param _function The aggregation function to be used.
         */
        Aggregator(std::string _name, Function _function)
            : INode(std::move(_name)), aggregatorFunction(std::move(_function)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.Constructor(std::string, Function)\n", this->name.c_str(), this);
        }

        /**
         * @brief Copy constructor.
         *
         * @param other The `DynamicAggregator` to copy from.
         */
        Aggregator(const Aggregator& other)
            : INode(other), aggregatorFunction(other.aggregatorFunction) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.CopyConstructor(DynamicAggregator)\n", this->name.c_str(), this);
        }

        /**
         * @brief Copy constructor with a new name.
         *
         * @param other The `DynamicAggregator` to copy from.
         * @param _name The new name for the aggregator.
         */
        Aggregator(const Aggregator& other, std::string _name)
            : INode(other, std::move(_name)), aggregatorFunction(other.aggregatorFunction) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.CopyConstructor(DynamicAggregator, std::string)\n", this->name.c_str(), this);
        }

        /**
         * @brief Move constructor.
         *
         * @param other The `DynamicAggregator` to move from.
         */
        Aggregator(Aggregator&& other) noexcept
            : INode(other), aggregatorFunction(std::move(other.aggregatorFunction)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.CopyConstructor(DynamicAggregator, std::string)\n", this->name.c_str(), this);
        }

        /**
         * @brief Destructor.
         */
        ~Aggregator() override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.Destructor()\n", this->name.c_str(), this);
        }

        /**
         * @brief Creates a copy of the current `DynamicAggregator`.
         *
         * @return A unique pointer to the cloned `DynamicAggregator`.
         */
        std::unique_ptr<INode> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<Aggregator>(*this);
        }

        /**
         * @brief Creates a copy of the current `DynamicAggregator` with a new name.
         *
         * @param _name The new name for the cloned aggregator.
         * @return A unique pointer to the cloned `DynamicAggregator`.
         */
        std::unique_ptr<INode> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] \"%s\" {%p}.clone(std::string)\n", this->name.c_str(), this);
            return make_unique<Aggregator>(*this, std::move(_name));
        }

    private:
        Function aggregatorFunction;

        /**
         * @brief Processes the input data and applies the aggregation function.
         *
         * @param input A vector of unique pointers to `IData` representing the input data.
         * @return A vector of unique pointers to `IData` containing the aggregated output.
         *
         * @throws std::bad_cast If the input data cannot be cast to the expected type.
         */
        std::vector<std::unique_ptr<IData>> processImpl(const std::vector<std::unique_ptr<IData>>& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicAggregator] {%p}.processImpl(std::vector<std::unique_ptr<IData>>&&)\n", this);

            std::vector<InputT> dataVector;
            dataVector.reserve(input.size());
            for (const auto& data : input) {
                auto castedData = dynamic_cast<const Data<InputT>*>(data.get());
                if (!castedData) {
                    throw std::bad_cast();
                }
                dataVector.push_back(castedData->value);
            }

            std::vector<std::unique_ptr<IData>> output;
            output.push_back(make_unique<Data<OutputT>>(std::move(aggregatorFunction(dataVector))));

            return output;
        }
    };
}

#endif //PIPEX_DYNAMICAGGREGATOR_HPP