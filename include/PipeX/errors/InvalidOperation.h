//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_CANCELOPERATION_H
#define PIPEX_CANCELOPERATION_H

#include <sstream>
#include <string>

#include "PipeXException.h"

namespace PipeX {
    /**
     * @brief Exception thrown when an invalid operation is attempted.
     *
     * Indicates that a requested operation cannot be performed in the current state.
     */
    class InvalidOperation final : public PipeXException {
    public:
        InvalidOperation(const std::string& operation, const std::string& message) : PipeXException(formatMessage(operation, message)) {}

    private:
        static std::string formatMessage(const std::string& operation, const std::string& message) {
            std::ostringstream oss;
            oss << "Invalid operation \"" << operation << "\": " << message;
            return oss.str();
        }
    };
}

#endif //PIPEX_CANCELOPERATION_H