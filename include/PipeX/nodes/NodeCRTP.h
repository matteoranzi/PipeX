//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_NODEBASE_H
#define PIPEX_NODEBASE_H

#include <string>
#include <vector>

#include "INode.h"
#include "PipeX/debug/pipex_print_debug.h"
#include "extended_cpp_standard/memory.h"
#include "PipeX/data/Data.h"
#include "PipeX/errors/TypeMismatchExpection.h"

namespace PipeX {
    template <typename Derived, typename InputT, typename OutputT>
    class NodeCRTP: public INode {
    protected:
        // Constructors inherited from INode
        using INode::INode;

        // Common logging helper
        void logLifeCycle(const std::string& event) const {
            PIPEX_PRINT_DEBUG_INFO("[%s] \"%s\" {%p}.%s()\n",
                typeName(),
                this->name().c_str(),
                this,
                event.c_str());
        }

        // Typename extraction (can override in derived)
        virtual std::string typeName() const {
            // Default implementation using typeid
            // Derived classes can override for custom names
            return typeid(Derived).name();
        }

        // Helper to extract typed data from IData vector
        std::vector<InputT> extractInputData(const std::vector<std::unique_ptr<IData>>& input) const {
            std::vector<InputT> extractedData;
            extractedData.reserve(input.size());
            for (const auto& data : input) {
                const auto rawData = data.get();
                auto castedData = dynamic_cast<const Data<InputT>*>(data.get());
                if (!castedData) {
                    throw TypeMismatchException(
                                        this->name,
                                        typeid(InputT),
                                        typeid(rawData)
                                    );
                }
                extractedData.push_back(castedData->value);
            }
            return extractedData;
        }

        // Helper to wrap output data into IData vector
        std::vector<std::unique_ptr<IData>> wrapOutputData(std::vector<OutputT>&& output) const {
            std::vector<std::unique_ptr<IData>> wrappedData;
            wrappedData.reserve(output.size());
            for (auto& item : output) {
                wrappedData.push_back(make_unique<Data<OutputT>>(std::move(item)));
            }
            return wrappedData;
        }

    public:
        using inputType = InputT;
        using outputType = OutputT;

        // Clone methods using CRTP
        std::unique_ptr<INode>  clone() const override {
            logLifeCycle("clone()");
            return make_unique<Derived>(static_cast<const Derived&>(*this));
        }

        std::unique_ptr<INode> clone(std::string _name) const override {
            logLifeCycle("clone(std::string)");
            return make_unique<Derived>(static_cast<const Derived&>(*this), std::move(name));
        }
    };
}

#endif //PIPEX_NODEBASE_H