//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_TRANSFORMER_H
#define PIPEX_TRANSFORMER_H
#include <functional>

#include "NodeCRTP.h"

namespace PipeX {
    /**
     * @class Transformer
     * @brief A specialized INode that transforms input data from one type to output data of another type.
     *
     * The Transformer class is a pipeline node that applies a user-defined transformation
     * function to convert data from InputT type to OutputT type. It processes multiple
     * input data items in a batch and produces corresponding output data.
    *  It supports cloning and copying, and provides debug information during its lifecycle.
     * The Transformer node inherits from NodeCRTP using the CRTP pattern, allowing compile-time
     * polymorphism while maintaining type safety.
     *
     * @tparam InputT The type of input data to be transformed
     * @tparam OutputT The type of output data after transformation
     *
     *  @code
     *  // Create a transformer that calculates the square of an integer
     *  Transformer<int, int> squareTransformer([](const int& x) { return x * x; });
     *  @endcode
     */
    template <typename InputT, typename OutputT>
    class Transformer: public NodeCRTP<Transformer<InputT, OutputT>, InputT, OutputT> {

        using Base = NodeCRTP<Transformer<InputT, OutputT>, InputT, OutputT>;

    public:
        /**
         * @brief Type alias for the transformation function.
         *
         * The function takes a `InputT` data and returns a transformed `OutputT` data.
         */
        using Function = std::function<OutputT(const InputT& data)>;

        /**
         * @brief Constructs a Transformer with a transformation function.
         * @param _function The function to apply to each input data item
         */
        explicit Transformer(Function _function) : Base(), transformerFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(Function)");
        }

        /**
         * @brief Constructs a named Transformer with a transformation function.
         * @param _name The name identifier for this transformer node
         * @param _function The function to apply to each input data item
         */
        Transformer(std::string _name, Function _function) : Base(std::move(_name)), transformerFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(std::string, Function)");
        }

        /**
         * @brief Copy constructor.
         * @param other The Transformer to copy from
         */
        Transformer(const Transformer& other) : Base(other), transformerFunction(other.transformerFunction) {
            this->logLifeCycle("CopyConstructor(const Transformer&)");
        }

        /**
         * @brief Copy constructor with name override.
         * @param other The Transformer to copy from
         * @param _name The name to assign to the new transformer
         */
        Transformer(const Transformer&other, std::string _name) : Base(other, std::move(_name)), transformerFunction(other.transformerFunction) {
            this->logLifeCycle("CopyConstructor(const Transformer&, std::string)");
        }

        /**
         * @brief Move constructor.
         * @param other The Transformer to move from
         */
        Transformer(Transformer&& other) noexcept : Base(other), transformerFunction(std::move(other.transformerFunction)) {
            this->logLifeCycle("MoveConstructor(Transformer&&)");
        }

        /**
         * @brief Destructor.
         */
        ~Transformer() override {
             this->logLifeCycle("Destructor()");
        }

        virtual bool isSource() const final { return  false; }
        virtual bool isSink() const final { return  false; }

    protected:
        /**
         * @brief Returns the type name of this node.
         *
         * Used for debugging and logging purposes to identify the node type.
         *
         * @return A string "Transformer" identifying this node type.
         */
        std::string typeName() const override {
            return "Transformer";
        }

    private:
        /// The transformation function applied to each input data item
        Function transformerFunction;

        /**
         * @brief Processes input data by applying the transformation function.
         *
         * Extracts input data from the IData wrapper, applies the transformation
         * function to each item, and wraps the results back into IData format.
         *
         * @param input Vector of input data wrapped in IData interface
         * @return Vector of transformed output data wrapped in IData interface
         */
        std::unique_ptr<std::vector<OutputT>> processImpl(std::unique_ptr<std::vector<InputT>>&& input) const override {
            this->logLifeCycle("processImpl(std::unique_ptr<std::vector<InputT>>&&)");

            auto output = make_unique<std::vector<OutputT>>();
            output->reserve(input->size());

            // Transform data
            for (const auto& data : *input) {
                output->push_back(transformerFunction(data));
            }
            return output;
        }
    };
}

#endif //PIPEX_TRANSFORMER_H