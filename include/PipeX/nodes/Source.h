//
// Created by Matteo Ranzi on 25/12/25.
//

#ifndef PIPEX_SOURCE_H
#define PIPEX_SOURCE_H

#include <functional>
#include "NodeCRTP.h"

namespace PipeX {
    template <typename T>
    class Source : public NodeCRTP<Source<T>, T, T> {

        using Base = NodeCRTP<Source<T>, T, T>;

    public:
        using Function = std::function<std::vector<T>()>;

        /**
         * @brief Constructs a Source node with a source function.
         * @param _function The function that returns initial data
         */
        explicit Source(Function _function) : Base(), sourceFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(Function)");
        }

        /**
         * @brief Constructs a named Source node with a source function.
         * @param _name The name identifier for this Source node
         * @param _function The function that returns initial data
         */
        Source(std::string _name, Function _function) : Base(std::move(_name)), sourceFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(std::string, Function)");
        }

        /**
         * @brief Copy constructor.
         * @param other The Source to copy from
         */
        Source(const Source& other) : Base(other), sourceFunction(other.sourceFunction) {
            this->logLifeCycle("CopyConstructor(const Source&)");
        }

        /**
         * @brief Copy constructor with name override.
         * @param other The Source to copy from
         * @param _name The name to assign to the new Source
         */
        Source(const Source&other, std::string _name) : Base(other, std::move(_name)), sourceFunction(other.sourceFunction) {
            this->logLifeCycle("CopyConstructor(const Source&, std::string)");
        }

        /**
         * @brief Move constructor.
         * @param other The Source to move from
         */
        Source(Source&& other) noexcept : Base(other), sourceFunction(std::move(other.sourceFunction)) {
            this->logLifecycle("MoveConstructor(Source&&)");
        }

        /**
         * @brief Destructor.
         */
        ~Source() override {
            this->logLifeCycle("Destructor()");
        }

    protected:
        std::string typeName() const override {
            return "Source";
        }

    private:
        Function sourceFunction;

        std::vector<std::unique_ptr<IData>> processImpl(const std::vector<std::unique_ptr<IData>>& input) const override {
            // input is ignored for Source nodes
            this->logLifeCycle("processImpl(std::vector<std::unique_ptr<IData>>&)");

            //Wrap output data
            return this->wrapOutputData(std::move(sourceFunction()));
        }
    };
}

#endif //PIPEX_SOURCE_H