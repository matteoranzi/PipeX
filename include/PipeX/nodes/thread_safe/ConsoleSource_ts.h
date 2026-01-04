//
// Created by Matteo Ranzi on 30/12/25.
//

#ifndef PIPEX_CONSOLESOURCE_H
#define PIPEX_CONSOLESOURCE_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <mutex>

#include "../../utils/Console_threadsafe_utils.h"
#include "PipeX/nodes/primitives/Source.h"


namespace PipeX {
    /**
     * @brief Thread-safe source node that reads data from the console.
     *
     * This node prompts the user to input a vector of data from the standard input in a thread-safe manner.
     * It requires the data type T to have an overloaded operator>> for std::istream.
     *
     * @tparam T The type of data to be read.
     */
    template <typename T>
    class ConsoleSource_ts final: public Source<T>, protected Console_threadsafe {
    public:
        explicit ConsoleSource_ts(std::string _description) : Source<T>([this]() -> std::vector<T> {return this->consoleInput();}), description(std::move(_description)) {
            this->logLifeCycle("Constructor(std::string)");
        }

        ConsoleSource_ts(std::string _name, std::string _description) : Source<T>(std::move(_name), [this]() -> std::vector<T> {return this->consoleInput();}), description(std::move(_description)) {
            this->logLifeCycle("Constructor(std::string, std::string)");
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

        std::vector<T> consoleInput() {
            size_t n;
            std::vector<T> inputData;

            // lock (i.e. console_mutex) is release when lock_guard goes out of scope
            // prevents console_mutex being locked if an in/out exception happens
            const std::lock_guard<std::mutex> lock(this->console_mutex);

            std::cout << "\n----------------------------------------" << std::endl;
            std::cout << "ConsoleSource - " << description << std::endl;
            std::cout << "Insert the number of elements to input: ";
            std::cin >> n;
            inputData.resize(n);
            for (size_t i = 0; i < n; i++) {
                std::cout << "Enter value #" << (i + 1) << ": ";
                std::cin >> inputData[i];
            }
            std::cout << "----------------------------------------\n" << std::endl;

            return inputData;
        }
    };
}

#endif //PIPEX_CONSOLESOURCE_H