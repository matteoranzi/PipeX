//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_NODEBASE_H
#define PIPEX_NODEBASE_H

#include <string>
#include <vector>

#include "INode.h"
#include "PipeX/debug/pipex_print_debug.h"
#include "extended_cpp_standard/memory.h"
#include "PipeX/data/Data.h"
#include "PipeX/errors/TypeMismatchExpection.h"

// TODO improve extraction/wrapping logic (currently each pass copies data multiple times, once for extraction and once for wrapping, for every node step in the pipeline)

namespace PipeX {
    /**
     * @brief CRTP base class for pipeline nodes with typed input/output
     *
     * This class uses the Curiously Recurring Template Pattern (CRTP) to provide
     * common functionality for pipeline nodes while maintaining type safety.
     *
     * @tparam Derived The derived class type (CRTP pattern)
     * @tparam InputT The input data type for this node
     * @tparam OutputT The output data type for this node
     */
    template <typename Derived, typename InputT, typename OutputT>
    class NodeCRTP: public INode {

    static_assert(std::is_copy_constructible<InputT>::value || std::is_move_constructible<InputT>::value, "InputT must be copyable or movable");
    static_assert(std::is_copy_constructible<OutputT>::value || std::is_move_constructible<OutputT>::value, "OutputT must be copyable or movable");

    protected:
        using inputType = InputT;
        using outputType = OutputT;

        // Constructors inherited from INode
        using INode::INode;

        /**
         * @brief Logs lifecycle events for debugging purposes
         *
         * @param event The lifecycle event name (e.g., "clone", "execute")
         */
        static void logLifeCycle(const std::string& event) {
            PIPEX_PRINT_DEBUG_INFO("[%s] \"%s\" {%p}.%s()\n",
                typeName(),
                this->name().c_str(),
                this,
                event.c_str());
        }

        /**
         * @brief Returns the type name of the derived class
         *
         * Default implementation uses RTTI. Derived classes can override
         * to provide custom human-readable names.
         *
         * @return The type name as a string
         */
        virtual std::string typeName() const {
            // Default implementation using typeid
            // Derived classes can override for custom names
            return typeid(Derived).name();
        }

        /**
         * @brief Extracts and validates typed input data from generic IData vector
         *
         * Performs dynamic casting and type validation. Throws TypeMismatchException
         * if any data element cannot be cast to InputT.
         *
         * @param input Vector of generic IData pointers
         * @return Vector of extracted typed input values
         * @throws TypeMismatchException if type validation fails
         */
        //Fixme: the solution with CRTP helper functions is cleaner but less efficient: a copy of the entire input is created (extracted) and stored
        std::vector<InputT> extractInputData(const std::vector<std::unique_ptr<IData>>& input) const {
            std::vector<InputT> extractedData;
            extractedData.reserve(input.size());
            for (const auto& data : input) {
                auto castedData = dynamic_cast<const Data<InputT>*>(data.get());
                if (!castedData) {
                    throw TypeMismatchException(
                                        this->name,
                                        typeid(InputT),
                                        typeid(data.get())
                                    );
                }
                extractedData.push_back(std::move(castedData->value));
            }
            return extractedData;
        }

        /**
         * @brief Wraps typed output data into generic IData vector
         *
         * Takes ownership of the output data via move semantics and wraps
         * each element in a Data<OutputT> container.
         *
         * @param output Vector of typed output values (moved)
         * @return Vector of wrapped IData pointers ready for pipeline transmission
         */
        //Fixme: the solution with CRTP helper functions is cleaner but less efficient: a copy of the entire input is created (wrapped) and stored
        std::vector<std::unique_ptr<IData>> wrapOutputData(std::vector<OutputT>&& output) const {
            std::vector<std::unique_ptr<IData>> wrappedData;
            wrappedData.reserve(output.size());
            for (auto& item : output) {
                wrappedData.push_back(make_unique<Data<OutputT>>(std::move(item)));
            }
            return wrappedData;
        }

    public:
        /**
         * @brief Creates a deep copy of this node
         *
         * Uses CRTP to ensure the correct derived type is cloned.
         * Logs the clone operation for debugging.
         *
         * @return Unique pointer to the cloned node
         */
        std::unique_ptr<INode>  clone() const override {
            logLifeCycle("clone()");
            return make_unique<Derived>(static_cast<const Derived&>(*this));
        }

        /**
         * @brief Creates a deep copy of this node with a new name
         *
         * Uses CRTP to ensure the correct derived type is cloned.
         * Logs the clone operation for debugging.
         *
         * @param _name The new name for the cloned node
         * @return Unique pointer to the cloned node with updated name
         */
        std::unique_ptr<INode> clone(std::string _name) const override {
            logLifeCycle("clone(std::string)");
            return make_unique<Derived>(static_cast<const Derived&>(*this), std::move(name));
        }
    };
}

#endif //PIPEX_NODEBASE_H