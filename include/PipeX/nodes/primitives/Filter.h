//
// Created by Matteo Ranzi on 21/12/25.
//

#ifndef PIPEX_FILTER_H
#define PIPEX_FILTER_H

#include <functional>

#include "NodeCRTP.h"
#include "PipeX/data/IData.h"
#include "my_extended_cpp_standard/my_memory.h"

namespace PipeX {
    /**
     * @class Filter
     * @brief A specialized INode that filters input data based on a predicate.
     *
     * This class processes input data and filters it using a user-defined predicate function.
     * It supports cloning and copying, and provides debug information during its lifecycle.
     * The Filter node inherits from NodeCRTP using the CRTP pattern, allowing compile-time
     * polymorphism while maintaining type safety.
     *
     * @tparam T The type of data to be filtered. Must be compatible with the predicate function.
     *
     * @note The Filter maintains type consistency by accepting input of type T and producing
     *       output of type T, only passing through elements that satisfy the predicate.
     *
     * @code
     * // Create a filter that only passes even numbers
     * Filter<int> evenFilter([](const int& x) { return x % 2 == 0; });
     * @endcode
     */
    template <typename T>
    class Filter: public NodeCRTP<Filter<T>, T, T> {

        using Base = NodeCRTP<Filter<T>, T, T>;

    public:
        /**
         * @typedef Predicate
         * @brief A function type that takes a constant reference to data of type T and returns a boolean.
         *
         * The predicate determines whether an element should be included in the filtered output.
         * Returns true to include the element, false to exclude it.
         */
        using Predicate = std::function<bool(const T& data)>;

        /**
         * @brief Constructs a Filter with a given predicate.
         *
         * Creates a Filter node with default name and the specified filtering predicate.
         *
         * @param _predicate The predicate function used to filter data. Moved into the object.
         *
         * @post The Filter is initialized and ready to process data.
         */
        explicit Filter(Predicate _predicate) : Base(), predicateFilter(std::move(_predicate)) {
            this->logLifeCycle("Constructor(Filter)");
        }

        /**
         * @brief Constructs a Filter with a name and a predicate.
         *
         * Creates a named Filter node with the specified filtering predicate.
         *
         * @param _name The name of the filter node for debugging and identification.
         * @param _predicate The predicate function used to filter data. Moved into the object.
         *
         * @post The Filter is initialized with the given name and ready to process data.
         */
        Filter(std::string _name, Predicate _predicate) : Base(std::move(_name)), predicateFilter(std::move(_predicate)) {
            this->logLifeCycle("Constructor(std::string, Predicate)");
        }

        /**
         * @brief Copy constructor.
         *
         * Creates a new Filter by copying another Filter instance, including its predicate.
         *
         * @param other The Filter instance to copy from.
         *
         * @post A new Filter is created with the same predicate as the original.
         */
        Filter(const Filter& other) : Base(other), predicateFilter(other.predicateFilter) {
            this->logLifeCycle("CopyConstructor(const Filter&)");
        }

        /**
         * @brief Copy constructor with a new name.
         *
         * Creates a new Filter by copying another Filter instance but with a different name.
         *
         * @param other The Filter instance to copy from.
         * @param _name The new name for the copied filter.
         *
         * @post A new Filter is created with the same predicate as the original but with the new name.
         */
        Filter(const Filter& other, std::string _name) : Base(other, std::move(_name)), predicateFilter(other.predicateFilter) {
            this->logLifeCycle("CopyConstructor(const Filter&, std::string)");
        }

        /**
         * @brief Move constructor.
         * @param other The Filter to move from
         */
        Filter(Filter&& other) noexcept : Base(std::move(other)) {
            this->logLifecycle("MoveConstructor(Filter&&)");
        }

        /**
         * @brief Destructor.
         *
         * Cleans up the Filter instance and logs its destruction.
         */
        ~Filter() override {
            this->logLifeCycle("Destructor()");
        }

        virtual bool isSource() const final { return  false; }
        virtual bool isSink() const final { return  false; }

    protected:
        /**
         * @brief Returns the type name of this node.
         *
         * Used for debugging and logging purposes to identify the node type.
         *
         * @return A string "Filter" identifying this node type.
         */
        std::string typeName() const override {
            return "Filter";
        }

    private:
        /// The predicate function used to determine which elements pass through the filter
        Predicate predicateFilter;

        /**
         * @brief Processes the input data and filters it based on the predicate.
         *
         * This method extracts data from the IData wrapper, applies the predicate to each
         * element, and only includes elements that satisfy the predicate in the output.
         *
         * @param input A vector of unique pointers to IData representing the input data.
         *
         * @return A vector of unique pointers to IData representing the filtered output data.
         *         Only elements where predicateFilter returns true are included.
         *
         * @throws PipeXTypeError If the input data cannot be cast to the expected type T.
         *
         * @note The output vector may be smaller than the input vector if elements are filtered out.
         * @note Memory for filtered-out elements is automatically released.
         */
        std::unique_ptr<IData> processImpl(const std::unique_ptr<IData>& input) const override {
            this->logLifeCycle("processImpl(std::vector<std::unique_ptr<IData>>&");

            // Extract input data using Base helper
            auto inputData = this->extractInputData(input);

            // Filter data based on predicate
            std::vector<T> outputData;
            outputData.reserve(inputData->size());
            for (const auto& data : *inputData) {
                if (predicateFilter(data)) {
                    outputData.push_back(std::move(data));
                }
            }

            // Wrap output data back into IData format using Base helper
            return this->wrapOutputData(make_unique<std::vector<T>>(std::move(outputData)));
        }
    };
}

#endif //PIPEX_FILTER_H