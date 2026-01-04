//
// Created by Matteo Ranzi on 04/01/26.
//

#ifndef PIPEX_PIPEX_IO_EXCEPTION_H
#define PIPEX_PIPEX_IO_EXCEPTION_H

#include <cstring>
#include <sstream>

#include "PipeXException.h"

namespace PipeX {
    /**
     * @class PipeX_IO_Exception
     * @brief Exception thrown when an Input/Output error occurs within the PipeX library.
     *
     * This class inherits from PipeXException and is used specifically to signal
     * failures related to I/O operations, such as Sink ans Source nodes failing to read from or write to files.
     */
    class PipeX_IO_Exception final : public PipeXException {
    public:
        /**
         * @brief Constructs a new PipeX_IO_Exception object.
         *
         * @param message A descriptive error message explaining the cause of the I/O exception.
         */
        explicit PipeX_IO_Exception(const std::string& message)
            : PipeXException(message) {}

    };
}

#endif //PIPEX_PIPEX_IO_EXCEPTION_H