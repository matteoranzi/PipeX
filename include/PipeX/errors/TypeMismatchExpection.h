//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_TYPEMISMATCHEXPECTION_H
#define PIPEX_TYPEMISMATCHEXPECTION_H

#include <stdexcept>
#include <string>
#include <ostream>
#include <sstream>

namespace PipeX {

    class TypeMismatchException final : public std::runtime_error {
    public:
        TypeMismatchException(
            const std::string& nodeName,
            const std::type_info& expected,
            const std::type_info& actual)
            : std::runtime_error(formatMessage(nodeName, expected, actual)),
              nodeName_(nodeName),
              expectedType_(&expected),
              actualType_(&actual)
        {}

        const std::string& getNodeName() const { return nodeName_; }
        const std::type_info& getExpectedType() const { return *expectedType_; }
        const std::type_info& getActualType() const { return *actualType_; }

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
