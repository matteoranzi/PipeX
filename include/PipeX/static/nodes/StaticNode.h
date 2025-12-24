//
// Created by Matteo Ranzi on 04/11/25.
//

#ifndef PIPEX_NODE_H
#define PIPEX_NODE_H

#include <vector>
#include <string>
#include <sstream>

#include "debug/print_debug.h"
#include "PipeX/debug/pipex_print_debug.h"

namespace PipeX {
    template <typename InputT, typename OutputT>
    class StaticNode{
    public:
        //TODO: create NodeBase to store nodes of different type in a single vector and use input_type/output_type to cast them back
        //Fixme: does it allow to remove duplication of DynamicPipeline?
        using input_type = InputT;
        using output_type = OutputT;
        // using input_vector = std::vector<input_type>;
        // using output_vector = std::vector<output_type>;

        const std::string name;



        virtual std::unique_ptr<StaticNode> clone() const = 0;
        virtual std::unique_ptr<StaticNode> clone(std::string name) const = 0;

        StaticNode () : name([this]() {
                                std::ostringstream oss;
                                oss << this;
                                return oss.str();
                            }()
                        ) {
            PIPEX_PRINT_DEBUG_INFO("[Node] \"%s\" {%p}.Constructor()\n", name.c_str(), this);
        }
        explicit StaticNode (std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[Node] \"%s\" {%p}.Constructor(std::string)\n", name.c_str(), this);
        }
        StaticNode (const StaticNode& other) : name(other.name + "_copy") {
            PIPEX_PRINT_DEBUG_INFO("[Node] \"%s\" {%p}.CopyConstructor()\n", name.c_str(), this);
        }
        StaticNode (const StaticNode& other, std::string _name) : name(std::move(_name)) {
            PIPEX_PRINT_DEBUG_INFO("[Node] \"%s\" {%p}.CopyConstructor(newName)\n", name.c_str(), this);
        }

        virtual ~StaticNode() {
            PIPEX_PRINT_DEBUG_INFO("[Node] \"%s\" {%p}.Destructor()\n", name.c_str(), this);
        };


        virtual std::vector<OutputT> process(const std::vector<InputT>&& input) const final {
            PIPEX_PRINT_DEBUG_INFO("[Node] \"%s\" {%p}.process(&&)\n", name.c_str(), this);
            return std::move(processImpl(std::move(input)));
        }
        virtual std::vector<OutputT> process(const std::vector<InputT>& input) const {
            PIPEX_PRINT_DEBUG_INFO("[Node] \"%s\" {%p}.process(&)\n", name.c_str(), this);
            return std::move(processImpl(std::move(input)));
        }


        virtual std::vector<OutputT> operator() (const std::vector<InputT>&& input) const final {
            PIPEX_PRINT_DEBUG_INFO("[Node] \"%s\" {%p}.Operator(&&)\n", name.c_str(), this);
            return this->process(std::move(input));
        }
        virtual std::vector<OutputT> operator() (const std::vector<InputT>& input) const final {
            PIPEX_PRINT_DEBUG_INFO("[Node] \"%s\" {%p}.Operator(&)\n", name.c_str(), this);
            return this->process(std::move(input));
        }



    private:
        // To be implemented by derived classes
        // This way, the base Node class guarantees that std::move is applied consistently,
        // and derived classes only focus on their processing logic without worrying about move semantics
        virtual std::vector<OutputT> processImpl(const std::vector<InputT>&& input) const {
            PRINT_DEBUG_ERROR("THIS METHOD HAS TO BE OVERRIDDEN IN DERIVED CLASSES!\n");
            throw std::logic_error("Node::processImpl(const std::vector<InputT>&&) not implemented");
        }
    };
}

#endif
