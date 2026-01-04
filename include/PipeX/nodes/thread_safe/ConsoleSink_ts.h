//
// Created by Matteo Ranzi on 31/12/25.
//

#ifndef PIPEX_CONSOLESINK_H
#define PIPEX_CONSOLESINK_H
#include <string>
#include <iostream>
#include <vector>
#include <mutex>

#include "../../utils/Console_threadsafe_utils.h"
#include "PipeX/nodes/primitives/Sink.h"

namespace PipeX {
    /**
     * @brief Thread-safe sink node that outputs data to the console.
     *
     * This node prints the received data vector to the standard output in a thread-safe manner.
     * It requires the data type T to have an overloaded operator<< for std::ostream.
     *
     * @tparam T The type of data to be printed.
     */
    template <typename T>
    class ConsoleSink_ts final: public Sink<T>, protected Console_threadsafe {
    public:
        explicit ConsoleSink_ts(std::string _description) : Sink<T>([this](const std::vector<T>& data) {
                return this->consoleOutput(data);
            }), description(std::move(_description)) {
            this->logLifeCycle("Constructor(std::string)");
        }

    protected:
        std::string typeName() const override {
            return "ConsoleSink_ts";
        }

    private:
        std::string description;

        void consoleOutput(const std::vector<T>& data) {
            // lock (i.e. console_mutex) is release when lock_guard goes out of scope
            // prevents console_mutex being locked if an in/out exception happens
            const std::lock_guard<std::mutex> lock(this->console_mutex);

            std::cout << "\n----------------------------------------" << std::endl;
            std::cout << "ConsoleSink - " << description << std::endl;
            std::cout << "Received data: [";
            for (const auto& item : data) {
                std::cout << item;
                if (&item != &data.back()) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;
            std::cout << "----------------------------------------\n" << std::endl;
        }
    };
}

#endif //PIPEX_CONSOLESINK_H