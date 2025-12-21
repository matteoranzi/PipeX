//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_DYNAMICNODE_HPP
#define PIPEX_DYNAMICNODE_HPP
#include "../data/GenericData.h"

namespace PipeX {
    class DynamicNode {
    public:
        virtual ~DynamicNode() = default;

        virtual std::vector<std::unique_ptr<GenericData>> process(std::vector<std::unique_ptr<GenericData>>& input) const final {
            PRINT_DEBUG_INFO("[DynamicNode] {%p}.process(std::vector<Data_*>&)\n", this);
            return processImpl(input);
        }

    private:
        virtual std::vector<std::unique_ptr<GenericData>> processImpl(std::vector<std::unique_ptr<GenericData>>& input) const {
            PRINT_DEBUG_ERROR("THIS METHOD HAS TO BE OVERRIDDEN IN DERIVED CLASSES!\n");
            throw std::logic_error("DynamicNode::processImpl(const std::vector<std::unique_ptr<Data_>>&&) not implemented");
        }
    };
}

#endif //PIPEX_DYNAMICNODE_HPP