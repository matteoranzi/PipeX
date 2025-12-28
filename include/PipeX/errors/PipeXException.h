//
// Created by Matteo Ranzi on 28/12/25.
//

#ifndef PIPEX_PIPEXEXCEPTION_H
#define PIPEX_PIPEXEXCEPTION_H

#include <stdexcept>
#include <string>

namespace PipeX {

    /**
     * @class PipeXException
     * @brief Base exception class for PipeX library.
     *
     * This class serves as the base for all exceptions thrown by the PipeX library.
     * It extends std::runtime_error to provide a consistent interface for error handling.
     */
    class PipeXException : public std::runtime_error {
    public:
        /**
         * @brief Constructs a PipeXException with a given error message.
         *
         * @param message The error message describing the exception.
         */
        explicit PipeXException(const std::string& message)
            : std::runtime_error("PipeX Library exception: " + message) {}
    };

} // namespace PipeX

#endif //PIPEX_PIPEXEXCEPTION_H