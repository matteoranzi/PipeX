//
// Created by Matteo Ranzi on 30/12/25.
//

#ifndef PIPEX_CONSOLESOURCE_H
#define PIPEX_CONSOLESOURCE_H

#include <iostream>
#include <string>
#include <utility>
#include "PipeX/nodes/thread_safe/Console_threadsafe.h"
#include "PipeX/nodes/primitives/Source.h"


namespace PipeX {
    template <typename T>
    class ConsoleSource_ts final: public Source<T>, protected Console_threadsafe {
    public:
        explicit ConsoleSource_ts(std::string _description) : Source<T>([this]() -> std::vector<T> {
                size_t n;
                std::vector<T> inputData;

                lockConsole();
                std::cout << "ConsoleSource - " << description << std::endl;
                std::cout << "Insert the number of elements to input: ";
                std::cin >> n;
                inputData.resize(n);
                for (size_t i = 0; i < n; i++) {
                    std::cout << "Enter value #" << (i + 1) << ": ";
                    std::cin >> inputData[i];
                }
                unlockConsole();

                return inputData;
            }), description(std::move(_description)) {
            this->logLifeCycle("Constructor(std::string)");
        }

        ConsoleSource_ts(const ConsoleSource_ts& other): Source<T>(other), description(other.description + " - copy") {
            this->logLifeCycle("CopyConstructor(const ConsoleSource&)");
        }

        ConsoleSource_ts(const ConsoleSource_ts& other, std::string _name, std::string  _description): Source<T>(other, std::move(_name)), description(std::move(_description)) {
            this->logLifeCycle("CopyConstructor(const ConsoleSource&, std::string)");
        }

        ConsoleSource_ts(ConsoleSource_ts&& other) noexcept : Source<T>(std::move(other)), description(std::move(other.description)) {
            this->logLifeCycle("MoveConstructor(ConsoleSource&)");
        }

        ~ConsoleSource_ts() override {
            this->logLifeCycle("Destructor()");
        }

    protected:
        std::string typeName() const override {
            return "ConsoleSource_ts";
        }

    private:
        std::string description;
    };
}

#endif //PIPEX_CONSOLESOURCE_H