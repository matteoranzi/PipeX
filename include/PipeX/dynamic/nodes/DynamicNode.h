//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_DYNAMICNODE_HPP
#define PIPEX_DYNAMICNODE_HPP
#include "../data/GenericData.h"

namespace PipeX {
    class DynamicNode {
    public:
        const std::string name;

        virtual std::unique_ptr<DynamicNode> clone() const = 0;
        virtual std::unique_ptr<DynamicNode> clone(std::string name) const = 0;

        DynamicNode() : name([this]()->std::string {
                                        std::ostringstream oss;
                                        oss << this;
                                        return oss.str();
                                    }()
                                ) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.Constructor()\n", name.c_str(), this);
        }

        explicit DynamicNode(std::string name) : name(std::move(name)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.Constructor(std::string)\n", name.c_str(), this);
        }

        DynamicNode(const DynamicNode& other) : name(other.name + "_copy") {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.CopyConstructor()\n", name.c_str(), this);
        }

        DynamicNode(const DynamicNode& other, std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.CopyConstructor(newName)\n", name.c_str(), this);
        }

        virtual ~DynamicNode() {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] \"%s\" {%p}.Destructor()\n", name.c_str(), this);
        }

        virtual std::vector<std::unique_ptr<GenericData>> process(std::vector<std::unique_ptr<GenericData>>& input) const final {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.process(std::vector<std::unique_ptr<GenericData>>&)\n", this);
            return std::move(processImpl(std::move(input)));
        }

        virtual std::vector<std::unique_ptr<GenericData>> process(std::vector<std::unique_ptr<GenericData>>&& input) const final {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.process(std::vector<std::unique_ptr<GenericData>>&&)\n", this);
            return std::move(processImpl(std::move(input)));
        }

        virtual std::vector<std::unique_ptr<GenericData>> operator() (std::vector<std::unique_ptr<GenericData>>& input) const final {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.Operator(std::vector<std::unique_ptr<GenericData>>&)\n", this);
            return this->process(std::move(input));
        }

        virtual std::vector<std::unique_ptr<GenericData>> operator() (std::vector<std::unique_ptr<GenericData>>&& input) const final {
            PIPEX_PRINT_DEBUG_INFO("[DynamicNode] {%p}.Operator(std::vector<std::unique_ptr<GenericData>>&&)\n", this);
            return this->process(std::move(input));
        }

    private:
        // To be implemented by derived classes
        // This way, the base Node class guarantees that std::move is applied consistently,
        // and derived classes only focus on their processing logic without worrying about move semantics
        virtual std::vector<std::unique_ptr<GenericData>> processImpl(std::vector<std::unique_ptr<GenericData>>&& input) const {
            PRINT_DEBUG_ERROR("THIS METHOD HAS TO BE OVERRIDDEN IN DERIVED CLASSES!\n");
            throw std::logic_error("DynamicNode::processImpl(const std::vector<std::unique_ptr<Data_>>&&) not implemented");
        }
    };
}

#endif //PIPEX_DYNAMICNODE_HPP