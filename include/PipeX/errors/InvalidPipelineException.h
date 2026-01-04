//
// Created by Matteo Ranzi on 28/12/25.
//

#ifndef PIPEX_INVALIDPIPELINEEXCEPTION_H
#define PIPEX_INVALIDPIPELINEEXCEPTION_H

#include <sstream>
#include <string>

#include "PipeXException.h"

namespace PipeX {
    /**
     * @brief Exception thrown when a pipeline configuration is invalid.
     *
     * Indicates issues such as missing source/sink nodes or invalid node connections.
     */
    class InvalidPipelineException : public PipeXException {
    public:
        const std::string pipelineName_;
        const std::string message_;

        InvalidPipelineException(const std::string& pipelineName, const std::string& message) : PipeXException(formatMessage(pipelineName, message)), pipelineName_(pipelineName), message_(message) {}

    private:
        static std::string formatMessage(const std::string& pipelineName, const std::string& message) {
            std::ostringstream oss;
            oss << "Invalid Pipeline \"" << pipelineName << "\": " << message;
            return oss.str();
        }
    };
}


#endif //PIPEX_INVALIDPIPELINEEXCEPTION_H