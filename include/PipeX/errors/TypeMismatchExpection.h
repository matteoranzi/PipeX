//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_TYPEMISMATCHEXPECTION_H
#define PIPEX_TYPEMISMATCHEXPECTION_H

#include <string>
#include <ostream>
#include <sstream>
#include <typeinfo>

#include "PipeXException.h"

namespace PipeX {
    /**
     * @brief Exception thrown when data type mismatch occurs.
     *
     * Indicates that a node received data of a type different from what it expected.
     */
    class TypeMismatchException final : public PipeXException {
    public:
        TypeMismatchException(
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
            oss << "[Node: " << nodeName << "] Type mismatch: "
                << "expected " << expected.name()
                << ", got " << actual.name();
            return oss.str();
        }
    };

} // namespace PipeX


#endif //PIPEX_TYPEMISMATCHEXPECTION_H
