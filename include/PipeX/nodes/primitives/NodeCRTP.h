//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_NODEBASE_H
#define PIPEX_NODEBASE_H

#include <string>
#include <vector>

#include "INode.h"
#include "PipeX/debug/pipex_print_debug.h"
#include "my_extended_cpp_standard/my_memory.h"
#include "PipeX/data/Data.h"
#include "PipeX/errors/TypeMismatchExpection.h"
#include "my_extended_cpp_standard/my_memory.h"
#include "../../utils/node_utils.h"

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

        std::unique_ptr<std::vector<InputT>> extractInputData(std::unique_ptr<IData>&& data) const {
            return extractData<InputT>(std::move(data), this->name);
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

    public:
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
            return make_unique<Derived>(static_cast<const Derived&>(*this), std::move(_name));
        }
    };
}

#endif //PIPEX_NODEBASE_H