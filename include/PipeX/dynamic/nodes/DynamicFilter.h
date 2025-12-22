//
// Created by Matteo Ranzi on 21/12/25.
//

#ifndef PIPEX_DYNAMICFILTER_H
#define PIPEX_DYNAMICFILTER_H

#include <functional>

#include "DynamicNode.h"
#include "extended_cpp_standard/memory.h"
#include "PipeX/debug/pipex_print_debug.h"

namespace PipeX {
    class DynamicFilter final: public DynamicNode {
    public:
        using Predicate = std::function<bool(const GenericData *)>;

        explicit DynamicFilter(Predicate _predicate) : predicate(std::move(_predicate)) {
            PRINT_DEBUG_INFO("[DynamicFilter] {%p}.Constructor(Predicate)\n", this);
        }
        DynamicFilter(std::string _name, Predicate _predicate) : DynamicNode(std::move(_name)), predicate(std::move(_predicate)) {
            PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.Constructor(std::string, Predicate)\n", this->name.c_str(), this);
        }
        DynamicFilter(const DynamicFilter& other) : DynamicNode(other), predicate(other.predicate) {
            PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.CopyConstructor(DynamicFilter)\n", this->name.c_str(), this);
        }
        DynamicFilter(const DynamicFilter&other, std::string _name) : DynamicNode(other, std::move(_name)), predicate(other.predicate) {
            PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.CopyConstructor(DynamicFilter, std::string)\n", this->name.c_str(), this);
        }
        ~DynamicFilter() override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.Destructor()\n", this->name.c_str(), this);
        }

        std::unique_ptr<DynamicNode> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<DynamicFilter>(*this);
        }

        std::unique_ptr<DynamicNode> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[DynamicFilter] \"%s\" {%p}.clone(std::string)\n", this->name.c_str(), this);
            return make_unique<DynamicFilter>(*this, std::move(_name));
        }

    private:
        Predicate predicate;

        std::vector<std::unique_ptr<GenericData>> processImpl(std::vector<std::unique_ptr<GenericData>>&& input) const override {
            PRINT_DEBUG_INFO("[DynamicFilter] {%p}.processImpl(std::vector<Data_*>&)\n", this);
            std::vector<std::unique_ptr<GenericData>> output;
            for (const auto& data : input) {
                if (predicate(data.get())) {
                    output.push_back(std::move(const_cast<std::unique_ptr<GenericData>&>(data)));
                }
            }
            return output;
        }
    };
}

#endif //PIPEX_DYNAMICFILTER_H