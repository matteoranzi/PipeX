// File: include/PipeX/dynamic/nodes/DynamicNode.h
//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_INODE_HPP
#define PIPEX_INODE_HPP

#include <memory>
#include <typeindex>
#include <sstream>
#include <string>

#include "PipeX/data/IData.h"
#include "PipeX/debug/pipex_print_debug.h"

namespace PipeX {
    /**
     * @brief Abstract base class representing a dynamic processing node.
     *
     * INode defines a uniform interface for nodes that accept a
     * \c std::unique_ptr<IData>, perform processing and
     * return a new \c std::unique_ptr<IData>.
     *
     * Derived classes must implement the clone methods and override
     * \c process to provide their processing logic.
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
            PIPEX_PRINT_DEBUG_INFO("[INode] \"%s\" {%p} :: Constructor()\n", name.c_str(), this);
        }

        /**
         * @brief Construct a node with an explicit name.
         *
         * @param name Custom name for the node (moved into the member).
         */
        explicit INode(std::string name) : name(std::move(name)) {
            PIPEX_PRINT_DEBUG_INFO("[INode] \"%s\" {%p} :: Constructor(std::string)\n", name.c_str(), this);
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
            PIPEX_PRINT_DEBUG_INFO("[INode] \"%s\" {%p} :: CopyConstructor()\n", name.c_str(), this);
        }

        /**
         * @brief Copy constructor that sets a provided name.
         *
         * @param other Ignored except to match signature semantics; the new
         *              instance receives \c _name as its name.
         * @param _name Name to assign to the new instance (moved).
         */
        INode(const INode& other, std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[INode] \"%s\" {%p} :: CopyConstructor(newName)\n", name.c_str(), this);
        }

        INode(INode&& other) noexcept : name(std::move(other.name)) {
            PIPEX_PRINT_DEBUG_INFO("[INode] \"%s\" {%p} :: MoveConstructor(INode&&)\n", name.c_str(), this);
        }

        /**
         * @brief Virtual destructor.
         *
         * Ensures derived destructors are called and emits debug information.
         */
        virtual ~INode() {
            PIPEX_PRINT_DEBUG_INFO("[INode] \"%s\" {%p} :: Destructor()\n", name.c_str(), this);
        }

        virtual std::unique_ptr<IData> process(std::unique_ptr<IData>&& input) = 0;

        virtual std::unique_ptr<IData> operator() (std::unique_ptr<IData>&& input) {
            PIPEX_PRINT_DEBUG_INFO("[INode] {%p} :: Operator()(std::unique_ptr<IData>&&)\n", this);
            return this->process(std::move(input));
        }

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

        std::unique_ptr<IData> inputData;
        std::unique_ptr<IData> outputData;
    };
}

#endif //PIPEX_INODE_HPP