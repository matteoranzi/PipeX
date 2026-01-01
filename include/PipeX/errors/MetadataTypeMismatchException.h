//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_METADATATYPEMISMATCHEXCEPTION_H
#define PIPEX_METADATATYPEMISMATCHEXCEPTION_H

#include <string>
#include <ostream>
#include <sstream>

#include "PipeXException.h"

namespace PipeX {
    class MetadataTypeMismatchException final : public PipeXException {
    public:
        explicit MetadataTypeMismatchException(
            const std::string& nodeName,
            const std::type_info& expected,
            const std::type_info& actual)
            : PipeXException(formatMessage(nodeName, expected, actual)),
              nodeName_(nodeName),
              expectedType_(&expected),
              actualType_(&actual)
        {}

    private:
        std::string nodeName_;
        const std::type_info* expectedType_;
        const std::type_info* actualType_;

        static std::string formatMessage(
            const std::string& nodeName,
            const std::type_info& expected,
            const std::type_info& actual)
        {
            std::ostringstream oss;
            oss << "[Node: " << nodeName << "] Metadata mismatch: "
                << "expected " << expected.name()
                << ", got " << actual.name();
            return oss.str();
        }
    };
}

#endif //PIPEX_METADATATYPEMISMATCHEXCEPTION_H