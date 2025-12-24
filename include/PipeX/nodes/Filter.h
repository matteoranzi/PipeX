//
// Created by Matteo Ranzi on 21/12/25.
//

#ifndef PIPEX_DYNAMICFILTER_H
#define PIPEX_DYNAMICFILTER_H

#include <functional>

#include "INode.h"
#include "PipeX/data/IData.h"
#include "extended_cpp_standard/memory.h"
#include "PipeX/debug/pipex_print_debug.h"
#include "PipeX/data/Data.h"


namespace PipeX {

    /**
     * @class Filter
     * @brief A specialized DynamicNode that filters input data based on a predicate.
     *
     * This class processes input data and filters it using a user-defined predicate function.
     * It supports cloning and copying, and provides debug information during its lifecycle.
     *
     * @tparam T The type of data to be filtered.
     */
    template <typename T>
    class Filter final: public INode {
    public:
        /**
         * @typedef Predicate
         * @brief A function type that takes a constant reference to data of type T and returns a boolean.
         */
        using Predicate = std::function<bool(const T& data)>;

        /**
         * @brief Constructs a DynamicFilter with a given predicate.
         * @param _predicate The predicate function used to filter data.
         */
        explicit Filter(Predicate _predicate) : predicateFilter(std::move(_predicate)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] {%p}.Constructor(Predicate)\n", this);
        }

        /**
         * @brief Constructs a DynamicFilter with a name and a predicate.
         * @param _name The name of the filter.
         * @param _predicate The predicate function used to filter data.
         */
        Filter(std::string _name, Predicate _predicate)
            : INode(std::move(_name)), predicateFilter(std::move(_predicate)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.Constructor(std::string, Predicate)\n", this->name.c_str(), this);
        }

        /**
         * @brief Copy constructor.
         * @param other The DynamicFilter instance to copy from.
         */
        Filter(const Filter& other)
            : INode(other), predicateFilter(other.predicateFilter) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.CopyConstructor(DynamicFilter)\n", this->name.c_str(), this);
        }

        /**
         * @brief Copy constructor with a new name.
         * @param other The DynamicFilter instance to copy from.
         * @param _name The new name for the copied filter.
         */
        Filter(const Filter& other, std::string _name)
            : INode(other, std::move(_name)), predicateFilter(other.predicateFilter) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.CopyConstructor(DynamicFilter, std::string)\n", this->name.c_str(), this);
        }

        /**
         * @brief Destructor.
         */
        ~Filter() override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.Destructor()\n", this->name.c_str(), this);
        }

        /**
         * @brief Creates a copy of the current DynamicFilter.
         * @return A unique pointer to the cloned DynamicFilter.
         */
        std::unique_ptr<INode> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<Filter>(*this);
        }

        /**
         * @brief Creates a copy of the current DynamicFilter with a new name.
         * @param _name The new name for the cloned filter.
         * @return A unique pointer to the cloned DynamicFilter.
         */
        std::unique_ptr<INode> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.clone(std::string)\n", this->name.c_str(), this);
            return make_unique<Filter>(*this, std::move(_name));
        }

    private:
        Predicate predicateFilter; ///< The predicate function used to filter data.

        /**
         * @brief Processes the input data and filters it based on the predicate.
         * @param input A vector of unique pointers to GenericData representing the input data.
         * @return A vector of unique pointers to GenericData representing the filtered output data.
         * @throws std::bad_cast If the input data cannot be cast to the expected type.
         */
        std::vector<std::unique_ptr<IData>> processImpl(const std::vector<std::unique_ptr<IData>>& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] {%p}.processImpl(std::vector<std::unique_ptr<GenericData>>&&)\n", this);
            std::vector<std::unique_ptr<IData>> output;
            output.reserve(input.size());
            for (const auto& data : input) {
                auto castedData = dynamic_cast<const Data<T>*>(data.get());
                if (!castedData) {
                    throw std::bad_cast();
                }

                if (predicateFilter(castedData->value)) {
                    output.push_back(make_unique<Data<T>>(std::move(castedData->value)));
                }
            }
            return output;
        }
    };
}

#endif //PIPEX_DYNAMICFILTER_H