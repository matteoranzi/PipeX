//
// Created by Matteo Ranzi on 28/12/25.
//

#ifndef PIPEX_NODENAMECONFLICTEXCEPTION_H
#define PIPEX_NODENAMECONFLICTEXCEPTION_H

#include <sstream>
#include <string>

#include "InvalidPipelineException.h"

namespace PipeX {
    /**
     * @brief Exception thrown when a node name conflict occurs.
     *
     * Indicates that a node with the same name already exists in the pipeline.
     */
    class NodeNameConflictException final : public InvalidPipelineException {
    public:
        NodeNameConflictException(const std::string& pipelineName, const std::string&nodeName) : InvalidPipelineException(pipelineName, formatMessage(pipelineName, nodeName)), nodeName_(nodeName), pipelineName_(pipelineName) {}

    private:
        const std::string pipelineName_;
        const std::string nodeName_;

        static std::string formatMessage(const std::string& pipelineName, const std::string& nodeName) {
            std::ostringstream oss;
            oss << "Node name conflict: A node with the name '" << nodeName << "' already exists in the pipeline " <<pipelineName << ".";
            return oss.str();
        }
    };
}

#endif //PIPEX_NODENAMECONFLICTEXCEPTION_H