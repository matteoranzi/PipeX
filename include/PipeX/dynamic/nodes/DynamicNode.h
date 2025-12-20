//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_DYNAMICNODE_HPP
#define PIPEX_DYNAMICNODE_HPP
#include "../data/Data_.h"

namespace PipeX {
    class DynamicNode {
    public:
        virtual ~DynamicNode() = default;

        virtual std::vector<Data_*>process(std::vector<Data_*>& data_vector) const final {
            PRINT_DEBUG_INFO("[DynamicNode] {%p}.process(std::vector<Data_*>&)\n", this);
            return processImpl(data_vector);
        }

    private:
        virtual std::vector<Data_*> processImpl(std::vector<Data_*>& input) const {
            PRINT_DEBUG_ERROR("THIS METHOD HAS TO BE OVERRIDDEN IN DERIVED CLASSES!\n");
            throw std::logic_error("DynamicNode::processImpl(const std::vector<std::unique_ptr<Data_>>&&) not implemented");
        }
    };
}

#endif //PIPEX_DYNAMICNODE_HPP