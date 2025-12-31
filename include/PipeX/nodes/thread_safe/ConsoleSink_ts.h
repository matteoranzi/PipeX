//
// Created by Matteo Ranzi on 31/12/25.
//

#ifndef PIPEX_CONSOLESINK_H
#define PIPEX_CONSOLESINK_H
#include <string>
#include <iostream>

#include "Console_threadsafe.h"
#include "PipeX/nodes/primitives/Sink.h"

namespace PipeX {
    template <typename T>
    class ConsoleSink_ts final: public Sink<T>, protected Console_threadsafe {
    public:
        explicit ConsoleSink_ts(std::string _description) : Sink<T>([this](const std::vector<T>& data) {
                lockConsole();
                std::cout << "ConsoleSink - " << description << std::endl;
                std::cout << "Received data: [";
                for (const auto& item : data) {
                    std::cout << item;
                    if (&item != &data.back()) {
                        std::cout << ", ";
                    }
                }
                std::cout << "]" << std::endl;
                unlockConsole();
            }), description(std::move(_description)) {
            this->logLifeCycle("Constructor(std::string)");
        }

    protected:
        std::string typeName() const override {
            return "ConsoleSink_ts";
        }

    private:
        std::string description;
    };
}

#endif //PIPEX_CONSOLESINK_H