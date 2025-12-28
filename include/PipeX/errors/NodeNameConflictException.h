//
// Created by Matteo Ranzi on 28/12/25.
//

#ifndef PIPEX_NODENAMECONFLICTEXCEPTION_H
#define PIPEX_NODENAMECONFLICTEXCEPTION_H
#include <sstream>
#include <stdexcept>
#include <string>

namespace PipeX {
    class NodeNameConflictException final : public std::runtime_error {
    public:
        const std::string pipelineName_;
        const std::string nodeName_;

        NodeNameConflictException(const std::string& pipelineName, const std::string&nodeName) : std::runtime_error(formatMessage(pipelineName, nodeName)), nodeName_(nodeName), pipelineName_(pipelineName) {}

    private:
        static std::string formatMessage(const std::string& pipelineName, const std::string& nodeName) {
            std::ostringstream oss;
            oss << "Node name conflict: A node with the name '" << nodeName << "' already exists in the pipeline " <<pipelineName << ".";
            return oss.str();
        }
    };
}

#endif //PIPEX_NODENAMECONFLICTEXCEPTION_H