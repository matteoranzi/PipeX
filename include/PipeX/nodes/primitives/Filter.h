//
// Created by Matteo Ranzi on 21/12/25.
//

#ifndef PIPEX_FILTER_H
#define PIPEX_FILTER_H

#include <functional>

#include "NodeCRTP.h"

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
    template <typename T, typename MetadataT = IMetadata>
    class Filter: public NodeCRTP<Filter<T, MetadataT>, T, T, MetadataT> {

        using Base = NodeCRTP<Filter, T, T, MetadataT>;

    public:
        /**
         * @typedef Predicate
         * @brief A function type that takes a constant reference to data of type T and returns a boolean.
         *
         * The predicate determines whether an element should be included in the filtered output.
         * Returns true to include the element, false to exclude it.
         */
        using Predicate = std::function<bool(const T& data)>;
        friend Base;


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
        Filter(Filter&& other) noexcept : Base(std::move(other)), predicateFilter(std::move(other.predicateFilter)) {
            this->logLifeCycle("MoveConstructor(Filter&&)");
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


        std::unique_ptr<std::vector<T>> processImpl(std::unique_ptr<std::vector<T>>&& input) const override {
            this->logLifeCycle("processImpl(std::unique_ptr<std::vector<InputT>>&&)");

            // Filter data based on predicate
            auto output = make_unique<std::vector<T>>();
            output->reserve(input->size());

            std::copy_if(
                std::make_move_iterator(input->begin()),
                std::make_move_iterator(input->end()),
                std::back_inserter(*output),
                predicateFilter
                );

            return output;
        }
    };
}

#endif //PIPEX_FILTER_H