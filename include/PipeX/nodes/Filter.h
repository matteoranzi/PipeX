//
// Created by Matteo Ranzi on 04/11/25.
//

#ifndef PIPEX_FILTER_H
#define PIPEX_FILTER_H

#include "Node.h"
#include "PipeX/debug/pipex_print_debug.h"

#include <vector>
#include <functional>
#include <string>


namespace PipeX {
    template <typename T>
    class Filter final : public Node<T, T> {
    public:
        using Predicate = std::function<bool(const T&&)>;

        explicit Filter (Predicate _predicate) : predicate(std::move(_predicate)) {
            PIPEX_PRINT_DEBUG_INFO("[Filter] {%s}.Constructor(Predicate)\n", this->name.c_str());
        }
        Filter (std::string _name, Predicate _predicate) : Node<T, T>(std::move(_name)), predicate(std::move(_predicate)) {
            PIPEX_PRINT_DEBUG_INFO("[Filter] {%s}.Constructor(std::string, Predicate)\n", this->name.c_str());
        }
        ~Filter() override {
            PIPEX_PRINT_DEBUG_INFO("[Filter] {%s}.Destructor()\n", this->name.c_str());
        };

    private:
        Predicate predicate;

        std::vector<T> processImpl(const std::vector<T>&& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[Filter] {%s}.processImpl(&&)\n", this->name.c_str());
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
