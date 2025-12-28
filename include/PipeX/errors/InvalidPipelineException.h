//
// Created by Matteo Ranzi on 28/12/25.
//

#ifndef PIPEX_INVALIDPIPELINEEXCEPTION_H
#define PIPEX_INVALIDPIPELINEEXCEPTION_H

#include <sstream>
#include <stdexcept>
#include <string>

#include "PipeXException.h"

namespace PipeX {
    class InvalidPipelineException final : public PipeXException {
    public:
        const std::string pipelineName_;
        const std::string message_;

        InvalidPipelineException(const std::string& pipelineName, const std::string& message) : PipeXException(formatMessage(pipelineName, message)), message_(message), pipelineName_(pipelineName) {}

    private:
        static std::string formatMessage(const std::string& pipelineName, const std::string& message) {
            std::ostringstream oss;
            oss << "Invalid Pipeline \"" << pipelineName << "\": " << message;
            return oss.str();
        }
    };
}


#endif //PIPEX_INVALIDPIPELINEEXCEPTION_H