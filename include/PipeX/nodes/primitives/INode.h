// File: include/PipeX/dynamic/nodes/DynamicNode.h
//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_INODE_HPP
#define PIPEX_INODE_HPP

#include <typeindex>
#include <sstream>

#include "PipeX/data/IData.h"
#include "PipeX/debug/pipex_print_debug.h"

//TODO sort node
//TODO nodes with pre-implemented processImpl e.g. pass-through node, logger node, etc.

namespace PipeX {
    /**
     * @brief Abstract base class representing a dynamic processing node.
     *
     * DynamicNode defines a uniform interface for nodes that accept a vector
     * of \c IData wrapped in \c std::unique_ptr, perform processing and
     * return a new vector of owned \c IData objects.
     *
     * Derived classes must implement the clone methods and override
     * \c processImpl to provide their processing logic. The public \c process
     * and call operator overloads are \c final to ensure consistent move
     * semantics handling at the base class level.
     */
    class INode {
    public:

        /**
         * @brief Create a deep copy of this node.
         *
         * Must be implemented by derived classes to return a heap-allocated
         * copy of the concrete node type.
         *
         * @return std::unique_ptr<DynamicNode> Owning pointer to the cloned node.
         */
        virtual std::unique_ptr<INode> clone() const = 0;

        /**
         * @brief Create a deep copy of this node with a specific name.
         *
         * Implementations should produce a clone whose \c name equals the
         * provided parameter.
         *
         * @param name The name to assign to the cloned node.
         * @return std::unique_ptr<DynamicNode> Owning pointer to the cloned node.
         */
        virtual std::unique_ptr<INode> clone(std::string name) const = 0;

        /**
         * @brief Default constructor.
         *
         * Generates a default name based on the object's pointer value and
         * emits debug information via \c PIPEX_PRINT_DEBUG_INFO.
         */
        INode() : name([this]()->std::string {
                                        std::ostringstream oss;
                                        oss << this;
                                        return oss.str();
                                    }()
                                ) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.Constructor()\n", name.c_str(), this);
        }

        /**
         * @brief Construct a node with an explicit name.
         *
         * @param name Custom name for the node (moved into the member).
         */
        explicit INode(std::string name) : name(std::move(name)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.Constructor(std::string)\n", name.c_str(), this);
        }

        /**
         * @brief Copy constructor.
         *
         * Copies the other's name and appends "_copy" to make the new node's
         * name distinct. Emits debug information.
         *
         * @param other The node to copy.
         */
        INode(const INode& other) : name(other.name + "_copy") {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.CopyConstructor()\n", name.c_str(), this);
        }

        /**
         * @brief Copy constructor that sets a provided name.
         *
         * @param other Ignored except to match signature semantics; the new
         *              instance receives \c _name as its name.
         * @param _name Name to assign to the new instance (moved).
         */
        INode(const INode& other, std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.CopyConstructor(newName)\n", name.c_str(), this);
        }

        INode(INode&& other) noexcept : name(std::move(other.name)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.MoveConstructor(INode&&)\n", name.c_str(), this);
        }

        /**
         * @brief Virtual destructor.
         *
         * Ensures derived destructors are called and emits debug information.
         */
        virtual ~INode() {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.Destructor()\n", name.c_str(), this);
        }

        /**
         * @brief Process input data (const lvalue reference overload).
         *
         * This function is \c final to centralize logging and move-semantics
         * handling. It forwards to \c processImpl implemented by derived
         * classes.
         *
         * @param input Immutable reference to a vector of owned IData.
         * @return std::vector<std::unique_ptr<IData>> Resulting vector
         *         of owned IData produced by the node.
         */
        // virtual std::vector<std::unique_ptr<IData>> process(const std::vector<std::unique_ptr<IData>>& input) const final {
        //     PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.process(std::vector<std::unique_ptr<IData>>&)\n", this);
        //
        //     return processImpl(input);
        // }

        //TODO rvalue, overload ()
        virtual std::unique_ptr<IData> process(const std::unique_ptr<IData> input) const final {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.process(const std::unique_ptr<IData>&)\n", this);
            return processImpl(input);
        }

        /**
         * @brief Process input data (rvalue overload).
         *
         * Accepts a rvalue vector permitting derived implementations to take
         * advantage of moved input objects if desired. The base class ensures
         * the same call path by forwarding to \c processImpl.
         *
         * @param input A rvalue vector of owned IData.
         * @return std::vector<std::unique_ptr<IData>> Resulting vector of owned IData.
         */
        // virtual std::vector<std::unique_ptr<IData>> process(std::vector<std::unique_ptr<IData>>&& input) const final {
        //     PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.process(std::vector<std::unique_ptr<IData>>&&)\n", this);
        //     return processImpl(input);
        // }

        /**
         * @brief Call operator forwarding to \c process (const lvalue overload).
         *
         * Provides a function-call style interface for executing the node.
         *
         * @param input Immutable reference to a vector of owned IData.
         * @return std::vector<std::unique_ptr<IData>> Resulting vector of owned IData.
         */
        // virtual std::vector<std::unique_ptr<IData>> operator() (const std::vector<std::unique_ptr<IData>>& input) const final {
        //     PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.Operator()(std::vector<std::unique_ptr<IData>>&)\n", this);
        //     return this->process(input);
        // }

        virtual std::unique_ptr<IData> operator() (const std::unique_ptr<IData> input) const final {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.Operator()(const std::unique_ptr<IData>&)\n", this);
            // return this->process(std::move(input));
            return processImpl(input);
        }

        /**
         * @brief Call operator forwarding to \c process (rvalue overload).
         *
         * @param input Rvalue vector of owned IData.
         * @return std::vector<std::unique_ptr<IData>> Resulting vector of owned IData.
         */
        // virtual std::vector<std::unique_ptr<IData>> operator() (std::vector<std::unique_ptr<IData>>&& input) const final {
        //     PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.Operator()(std::vector<std::unique_ptr<IData>>&&)\n", this);
        //     return this->process(std::move(input));
        // }

        virtual bool isSource() const { return  false; }
        virtual bool isSink() const { return  false; }

        std::string getName() const { return name; }

    protected:
        /**
         * @brief Read-only identifier for the node.
         *
         * By default, the name is generated from the object's pointer value.
         * Derived classes or callers can supply a custom name via constructors.
         */
        std::string name;

    private:
        /**
         * @brief Implementation hook for derived classes to perform processing.
         *
         * Derived classes must override this method to implement their logic.
         * The base implementation reports an error and throws \c std::logic_error.
         *
         * Implementations receive a const reference to the input vector. If a
         * derived class needs to take ownership of elements, it may perform
         * explicit moves from the referenced vector.\n
         *
         * @param input Immutable reference to the inputs.
         * @return std::vector<std::unique_ptr<IData>> Processed outputs.
         *
         * @throws std::logic_error If not overridden by a derived class.
         * @note This method is intentionally private and virtual: public
         *       forwarding methods ensure consistent pre/post behaviour
         *       (e.g. logging) while derived classes only implement the core logic.
         */
        // virtual std::vector<std::unique_ptr<IData>> processImpl(const std::vector<std::unique_ptr<IData>>& input) const {
        //     PIPEX_PRINT_DEBUG_ERROR("'DynamicNode::processImpl' METHOD HAS TO BE OVERRIDDEN IN DERIVED CLASSES!\n");
        //     throw std::logic_error("DynamicNode::processImpl(const std::vector<std::unique_ptr<IData>>&) not implemented");
        // }

        virtual std::unique_ptr<IData> processImpl(const std::unique_ptr<IData>& input) const {
            PIPEX_PRINT_DEBUG_ERROR("'DynamicNode::processImpl' METHOD HAS TO BE OVERRIDDEN IN DERIVED CLASSES!\n");
            throw std::logic_error("DynamicNode::processImpl(const std::unique_ptr<IData>&) not implemented");
        }
    };
}

#endif //PIPEX_INODE_HPP