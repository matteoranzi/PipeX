//
// Created by Matteo Ranzi on 25/12/25.
//

#ifndef PIPEX_SINK_H
#define PIPEX_SINK_H

#include <functional>

#include "NodeCRTP.h"

namespace PipeX {
    template <typename T>
    class Sink: public NodeCRTP<Sink<T>, T, T> {

        using Base = NodeCRTP<Sink<T>, T, T>;

    public:
        using Function = std::function<void(std::vector<T>&)>;

        /**
         * @brief Constructs a Sink node with a sink function.
         * @param _function The function that consumes final data
         */
        explicit Sink(Function _function) : Base(), sinkFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(Function)");
        }

        /**
         * @brief Constructs a named Sink node with a sink function.
         * @param _name The name identifier for this Sink node
         * @param _function The function that consumes final data
         */
        Sink(std::string _name, Function _function) : Base(std::move(_name)), sinkFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(std::string, Function)");
        }

        /**
         * @brief Copy constructor.
         * @param other The Sink to copy from
         */
        Sink(const Sink& other) : Base(other), sinkFunction(other.sinkFunction) {
            this->logLifeCycle("CopyConstructor(const Sink&)");
        }

        /**
         * @brief Copy constructor with name override.
         * @param other The Sink to copy from
         * @param _name The name to assign to the new Sink
         */
        Sink(const Sink&other, std::string _name) : Base(other, std::move(_name)), sinkFunction(other.sinkFunction) {
            this->logLifeCycle("CopyConstructor(const Sink&, std::string)");
        }

        /**
         * @brief Move constructor.
         * @param other The Sink to move from
         */
        Sink(Sink&& other) noexcept : Base(other), sinkFunction(std::move(other.sinkFunction)) {
            this->logLifecycle("MoveConstructor(Sink&&)");
        }

        /**
         * @brief Destructor.
         */
        ~Sink() override {
            this->logLifeCycle("Destructor()");
        }

        bool isSink() const override final { return true; }

    protected:
        std::string typeName() const override {
            return "Sink";
        }

    private:
        Function sinkFunction;

        std::unique_ptr<IData> processImpl(const std::unique_ptr<IData>& input) const override {
            this->logLifeCycle("processImpl(std::vector<std::unique_ptr<IData>>&)");

            // Extract input data
            auto inputData = this->extractInputData(input);

            // Apply sink function
            sinkFunction(*inputData);

            // Sink produces no output
            return {};
        }
    };
}

#endif //PIPEX_SINK_H