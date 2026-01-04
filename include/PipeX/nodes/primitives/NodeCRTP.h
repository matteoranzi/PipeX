//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_NODEBASE_H
#define PIPEX_NODEBASE_H

#include <string>
#include <vector>
#include <typeinfo>
#include <memory>
#include <type_traits>

#include "INode.h"
#include "PipeX/debug/pipex_print_debug.h"
#include "my_extended_cpp_standard/my_memory.h"
#include "PipeX/data/Data.h"
#include "PipeX/errors/TypeMismatchExpection.h"
#include "PipeX/utils/node_utils.h"
#include "PipeX/errors/MetadataTypeMismatchException.h"
#include "PipeX/errors/InvalidOperation.h"

namespace PipeX {
    /**
     * @brief CRTP base class for pipeline nodes with typed input/output.
     *
     * This class uses the Curiously Recurring Template Pattern (CRTP) to provide
     * common functionality for pipeline nodes while maintaining type safety.
     * It handles the extraction of typed data from the generic `IData` input
     * and the wrapping of typed output data back into `IData`.
     *
     * @tparam Derived The derived class type (CRTP pattern).
     * @tparam InputT The input data type for this node.
     * @tparam OutputT The output data type for this node.
     * @tparam MetadataT The metadata type expected by this node (default: IMetadata).
     */
    template <typename Derived, typename InputT, typename OutputT, typename MetadataT = IMetadata>
    class NodeCRTP: public INode {

    static_assert(std::is_copy_constructible<InputT>::value || std::is_move_constructible<InputT>::value, "InputT must be copyable or movable");
    static_assert(std::is_copy_constructible<OutputT>::value || std::is_move_constructible<OutputT>::value, "OutputT must be copyable or movable");
    static_assert(std::is_destructible<InputT>::value, "InputT must be destructible");

    protected:

        // Constructors inherited from INode
        using INode::INode;

        /**
         * @brief Logs lifecycle events for debugging purposes
         *
         * @param e_event The lifecycle event name (e.g., "clone", "execute")
         */
        void logLifeCycle(const std::string& e_event) const {
            PIPEX_PRINT_DEBUG_INFO("[%s] \"%s\" {%p}.%s()\n",
                typeName().c_str(),
                this->name.c_str(),
                this,
                e_event.c_str());
        }

        std::unique_ptr<IData> wrapOutputData(std::unique_ptr<std::vector<OutputT>>&& data) const {
            return wrapData<OutputT>(std::move(data));
        }

        std::unique_ptr<std::vector<InputT>> extractInputData(const std::unique_ptr<IData>& data) const {
            try {
                return extractData<InputT>(std::move(data), this->name);
            } catch (TypeMismatchException& e) {
                PIPEX_PRINT_DEBUG_ERROR("[%s] \"%s\" {%p}.extractInputData() -> TypeMismatchException: %s\n",
                    typeName().c_str(),
                    this->name.c_str(),
                    this,
                    e.what());
                throw;
            }
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

        virtual void preProcessHook() const {
            // Default implementation does nothing
            // Derived classes can override to add pre-processing logic
        }

        virtual void postProcessHook() const {
            outputData->metadata = inputData->metadata; // by default, propagate input metadata to output
        }

        // Derived classes must implement this method for processing logic. derived classes' procesImpl is called via CRTP compile-time polymorphism, not virtual dispatch.
        virtual std::unique_ptr<std::vector<OutputT>> processImpl(std::unique_ptr<std::vector<InputT>>&& input) const = 0;

        std::shared_ptr<MetadataT> getMetadata() const {
            this->logLifeCycle("getMetadata()");

            if (this->inputData) {
                if (!this->inputData->metadata) {
                    PIPEX_PRINT_DEBUG_ERROR("[%s] \"%s\" {%p} :: getMetadata() -> No metadata available in data\n", typeName().c_str(), this->getName().c_str(), this);
                    throw InvalidOperation("NodeCRTP::getTypedMetadata", "No metadata available in data");
                }

                const auto typedMetadata = std::dynamic_pointer_cast<MetadataT>(this->inputData->metadata);
                if (!typedMetadata) {
                    PIPEX_PRINT_DEBUG_ERROR("[%s] \"%s\" {%p} :: getMetadata() -> MetadataTypeMismatchException: expected metadata type %s, but got %s\n",
                        typeName().c_str(),
                        this->getName().c_str(),
                        this,
                        typeid(MetadataT).name(),
                        typeid(this->inputData->metadata.get()).name());
                    throw MetadataTypeMismatchException(this->getName(), typeid(MetadataT), typeid(this->inputData->metadata.get()));
                }

                return typedMetadata;
            } else {
                PIPEX_PRINT_DEBUG_ERROR("[%s] \"%s\" {%p} :: getMetadata() -> No data available to extract metadata from\n", typeName().c_str(), this->getName().c_str(), this);
                throw InvalidOperation("NodeCRTP::getTypedMetadata", "No data available to extract metadata from");
            }
        }

    public:

        std::unique_ptr<IData> process(std::unique_ptr<IData>&& input) override {
            logLifeCycle("process(std::unique_ptr<IData>&&)");
            this->inputData = std::move(input); // store input data for potential later use (e.g., logging, debugging, etc.)

            static_cast<Derived const*>(this)->preProcessHook();

            auto extractedInput = extractInputData(this->inputData);
            auto outputData = static_cast<Derived const*>(this)->processImpl(std::move(extractedInput)); // CRTP compile-time polymorphism
            this->outputData = wrapOutputData(std::move(outputData));

            static_cast<Derived const*>(this)->postProcessHook();


            return std::move(this->outputData);
        }

        std::unique_ptr<INode> clone() const override {
            logLifeCycle("clone()");
            return extended_std::make_unique<Derived>(static_cast<const Derived&>(*this));
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
            return extended_std::make_unique<Derived>(static_cast<const Derived&>(*this), std::move( _name));
        }
    };
}

#endif //PIPEX_NODEBASE_H

