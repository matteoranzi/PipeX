//
// Created by Matteo Ranzi on 04/11/25.
//

#pragma once

#include <PipeX/nodes/Node.h>
#include <PipeX/debug/pipex_print_debug.h>

#include <vector>
#include <functional>
#include <string>


namespace PipeX {
    template <typename T>
    class Filter final : public Node<T, T> {
    public:
        using Predicate = std::function<bool(const T&)>;

        explicit Filter (Predicate _predicate) : predicate(std::move(_predicate)) {
            PIPEX_PRINT_DEBUG_INFO("[Filter] {%s}.Constructor(Predicate)\n", this->name.c_str());
        }
        Filter (std::string _name, Predicate _predicate) : Node<T, T>(std::move(_name)), predicate(std::move(_predicate)) {
            PIPEX_PRINT_DEBUG_INFO("[Filter] {%s}.Constructor(std::string, Predicate)\n", this->name.c_str());
        }
        ~Filter() override = default;

        std::vector<T> process(const std::vector<T>& input) const override {
            PIPEX_PRINT_DEBUG_INFO("[Filter] {%s}.process()\n", this->name.c_str());
            std::vector<T> output;
            for (const auto& item : input) {
                if (predicate(item)) {
                    output.push_back(item);
                }
            }
            return output;
        }

    private:
        Predicate predicate;
    };
} // PipeX