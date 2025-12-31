//
// Created by Matteo Ranzi on 30/12/25.
//

#ifndef PIPEX_CONSOLESOURCE_H
#define PIPEX_CONSOLESOURCE_H

#include <iostream>
#include <pthread.h>

#include "PipeX/nodes/thread_safe/console_threadsafe.h"
#include "PipeX/nodes/primitives/Source.h"


namespace PipeX {
    extern pthread_mutex_t console_mutex;

    template <typename T>
    class ConsoleSource_ts final: public Source<T> {
    public:
        explicit ConsoleSource_ts(std::string _description) : Source<T>(), description(std::move(_description)) {
            this->logLifecycle("Constructor(std::string)");

            this->sourceFunction = [this]() -> std::vector<T> {
                size_t n;
                std::vector<T> inputData;

                pthread_mutex_lock(&console_mutex);
                std::cout << "ConsoleSource - " << description << std::endl;
                std::cout << "Insert the number of elements to input: ";
                std::cin >> n;
                inputData.resize(n);
                for (size_t i = 0; i < n; i++) {
                    std::cout << "Enter value #" << (i + 1) << ": ";
                    std::cin >> inputData[i];
                }
                pthread_mutex_unlock(&console_mutex);

                return inputData;
            };
        }

        ConsoleSource_ts(const ConsoleSource_ts& other): Source<T>(other), description(other.description + " - copy") {
            this->logLifecycle("CopyConstructor(const ConsoleSource&)");
        }

        ConsoleSource_ts(const ConsoleSource_ts& other, std::string _name, const std::string& _description): Source<T>(other, std::move(_name)), description(_description) {
            this->logLifecycle("CopyConstructor(const ConsoleSource&, std::string)");
        }

        ConsoleSource_ts(ConsoleSource_ts&& other) noexcept : Source<T>(std::move(other)), description(std::move(other.description)) {
            this->logLifecycle("MoveConstructor(ConsoleSource&)");
        }

        ~ConsoleSource_ts() override {
            this->logLifecycle("Destructor()");
        }

    protected:
        std::string typeName() const override {
            return "ConsoleSource";
        }

    private:
        std::string description;
    };
}

#endif //PIPEX_CONSOLESOURCE_H