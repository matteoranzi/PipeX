//
// Created by Matteo Ranzi on 28/12/25.
//

#ifndef PIPEX_PIPELINENAMECONFLICTEXCEPTION_H
#define PIPEX_PIPELINENAMECONFLICTEXCEPTION_H

#include <sstream>
#include <string>

#include "PipeXException.h"

namespace PipeX {
    /**
     * @brief Exception thrown when a node name conflict occurs.
     *
     * Indicates that a node with the same name already exists in the pipeline.
     */
    class PipelineNameConflictException final : public PipeXException {
    public:
        explicit PipelineNameConflictException(const std::string& pipelineName) : PipeXException(formatMessage(pipelineName)), pipelineName_(pipelineName) {}

    private:
        const std::string pipelineName_;

        static std::string formatMessage(const std::string& pipelineName) {
            std::ostringstream oss;
            oss << "Pipeline name conflict: A pipeline with the name '" << pipelineName << " already exists in the PipeXEngine.";
            return oss.str();
        }
    };
}

#endif //PIPEX_PIPELINENAMECONFLICTEXCEPTION_H