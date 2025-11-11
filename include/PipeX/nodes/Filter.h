//
// Created by Matteo Ranzi on 04/11/25.
//

#ifndef PIPEX_FILTER_H
#define PIPEX_FILTER_H

#include "Node.h"
#include "PipeX/debug/pipex_print_debug.h"

#include "extended_cpp_standard/memory.h"

#include <vector>
#include <functional>
#include <string>


namespace PipeX {
    template <typename T>
    class Filter final : public Node<T, T> {
    public:
        using Predicate = std::function<bool(const T&&)>;

        explicit Filter (Predicate _predicate) : predicate(std::move(_predicate)) {
            PIPEX_PRINT_DEBUG_INFO("[Filter] \"%s\" {%p}.Constructor(Predicate)\n", this->name.c_str(), this);
        }
        Filter (std::string _name, Predicate _predicate) : Node<T, T>(std::move(_name)), predicate(std::move(_predicate)) {
            PIPEX_PRINT_DEBUG_INFO("[Filter] \"%s\" {%p}.Constructor(std::string, Predicate)\n", this->name.c_str(), this);
        }
        Filter (const Filter& other) : Node<T, T>(other), predicate(other.predicate) {
            PIPEX_PRINT_DEBUG_INFO("[Filter] \"%s\" {%p}.CopyConstructor()\n", this->name.c_str(), this);
        }
        Filter (const Filter& other, std::string _name) : Node<T, T>(other, std::move(_name)), predicate(other.predicate) {
            PIPEX_PRINT_DEBUG_INFO("[Filter] \"%s\" {%p}.CopyConstructor()\n", this->name.c_str(), this);
        }
        ~Filter() override {
            PIPEX_PRINT_DEBUG_INFO("[Filter] \"%s\" {%p}.Destructor()\n", this->name.c_str(), this);
        }

        std::unique_ptr<Node<int,int>> clone() const override {
            PIPEX_PRINT_DEBUG_INFO("[Filter] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<Filter>(*this);
        }
        std::unique_ptr<Node<int,int>> clone(std::string _name) const override {
            PIPEX_PRINT_DEBUG_INFO("[Filter] \"%s\" {%p}.clone()\n", this->name.c_str(), this);
            return make_unique<Filter>(*this, std::move(_name));
        }

    private:
        Predicate predicate;

        std::vector<T> processImpl(const std::vector<T>&& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[Filter] \"%s\" {%p}.processImpl(&&)\n", this->name.c_str(), this);
            std::vector<T> output;
            for (const auto& item : input) {
                if (predicate(std::move(item))) {
                    output.push_back(item);
                }
            }
            return output;
        }
    };
} // PipeX

#endif
