//
// Created by Matteo Ranzi on 30/12/25.
//

#ifndef PIPEX_CONSOLESOURCE_H
#define PIPEX_CONSOLESOURCE_H

#include <iostream>
#include <pthread.h>

#include "PipeX/nodes/primitives/Source.h"

namespace PipeX {
    template <typename T>
    class ConsoleSource: public Source<T> {
    public:
        explicit ConsoleSource(std::string _description) : Source<T>(), description(std::move(_description)) {
            this->logLifecycle("Constructor(std::string)");

            this->sourceFunction = [this]() -> std::vector<T> {
                size_t n;
                std::vector<T> inputData;

                pthread_mutex_lock(&mutex);
                std::cout << "ConsoleSource - " << description << std::endl;
                std::cout << "Insert the number of elements to input: ";
                std::cin >> n;
                inputData.resize(n);
                for (size_t i = 0; i < n; i++) {
                    std::cout << "Enter value #" << (i + 1) << ": ";
                    std::cin >> inputData[i];
                }
                pthread_mutex_unlock(&mutex);

                return inputData;
            };
        }

        ConsoleSource(const ConsoleSource& other): Source<T>(other), description(other.description + " - copy") {
            this->logLifecycle("CopyConstructor(const ConsoleSource&)");
        }

        ConsoleSource(const ConsoleSource& other, std::string _name, std::string _descrption): Source<T>(other, std::move(_name)), description(_descrption) {
            this->logLifecycle("CopyConstructor(const ConsoleSource&, std::string)");
        }

        ConsoleSource(ConsoleSource&& other) noexcept : Source<T>(std::move(other)), description(std::move(other.description)) {
            this->logLifecycle("MoveConstructor(ConsoleSource&)");
        }

        ~ConsoleSource() override {
            this->logLifecycle("Destructor()");
        }

    protected:
        std::string typeName() const override {
            return "ConsoleSource";
        }

    private:
        static pthread_mutex_t mutex;
        std::string description;
    };

    template <typename T>
    pthread_mutex_t ConsoleSource<T>::mutex = PTHREAD_MUTEX_INITIALIZER;
}

#endif //PIPEX_CONSOLESOURCE_H