//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_PROCESSOR_H
#define PIPEX_PROCESSOR_H

#include <functional>
#include "NodeCRTP.h"

namespace PipeX {
    /**
     * @class Processor
     * @brief A specialized INode that processes entire data vectors at once.
     *
     * This class processes a complete vector of input data and produces
     * a complete vector of output data. Useful for operations requiring
     * access to all elements simultaneously (e.g., sorting, shuffling).
     *
     * Unlike other node types:
     * - Transformer: one-to-one mapping (single input → single output)
     * - Filter: many-to-many with potential reduction (removes elements)
     * - Aggregator: many-to-one (reduces to single output)
     *
     * Processor is many-to-many with full access to the entire vector,
     * making it ideal for operations that require the complete dataset.
     *
     * @tparam T The type of data to be processed.
     *
     * @code
     * // Create a processor that sorts integers
     * Processor<int> sortProcessor([](std::vector<int> data) {
     *     std::sort(data.begin(), data.end());
     *     return data;
     * });
     * @endcode
     */
    template <typename T, typename MetadataT = IMetadata>
    class Processor: public NodeCRTP<Processor<T, MetadataT>, T, T, MetadataT> {
        using Base = NodeCRTP<Processor, T, T, MetadataT>;

    public:
        /// Function type that accepts a vector of T and returns a vector of T
        using Function = std::function<std::vector<T>(std::vector<T>& data)>;

        /**
         * @brief Constructs a Processor with the given function.
         * @param _function The processing function to apply to the data vector.
         */
        explicit Processor(Function _function)
            : Base(), processorFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(Function)");
        }

        /**
         * @brief Constructs a named Processor with the given function.
         * @param _name The name of the processor node.
         * @param _function The processing function to apply to the data vector.
         */
        Processor(std::string _name, Function _function)
            : Base(std::move(_name)), processorFunction(std::move(_function)) {
            this->logLifeCycle("Constructor(std::string, Function)");
        }

        /**
         * @brief Copy constructor.
         * @param other The Processor to copy from.
         */
        Processor(const Processor& other)
            : Base(other), processorFunction(other.processorFunction) {
            this->logLifeCycle("CopyConstructor(const Processor&)");
        }

        /**
         * @brief Copy constructor with custom name.
         * @param other The Processor to copy from.
         * @param _name The new name for the copied processor.
         */
        Processor(const Processor& other, std::string _name)
            : Base(other, std::move(_name)), processorFunction(other.processorFunction) {
            this->logLifeCycle("CopyConstructor(const Processor&, std::string)");
        }

        /**
         * @brief Move constructor.
         * @param other The Processor to move from.
         */
        Processor(Processor&& other) noexcept
            : Base(std::move(other)), processorFunction(std::move(other.processorFunction)) {
            this->logLifeCycle("MoveConstructor(Processor&&)");
        }

        /**
         * @brief Destructor.
         */
        ~Processor() override {
            this->logLifeCycle("Destructor()");
        }

        virtual bool isSource() const final { return  false; }
        virtual bool isSink() const final { return  false; }

    protected:
        /**
         * @brief Returns the type name of this node.
         * @return The string "Processor".
         */
        std::string typeName() const override {
            return "Processor";
        }

    private:
        /// The processing function that operates on the entire data vector
        Function processorFunction;

        /**
         * @brief Implements the processing logic for this node.
         *
         * Extracts all input data from the IData wrappers, applies the
         * processor function to the complete vector, and wraps the output
         * back into IData objects.
         *
         * @param input Vector of input data wrapped in IData objects.
         * @return Vector of processed data wrapped in IData objects.
         */
        std::unique_ptr<std::vector<T>> processImpl(std::unique_ptr<std::vector<T>>&& input) const  {
            this->logLifeCycle("processImpl(std::unique_ptr<std::vector<InputT>>&&)");

            return make_unique<std::vector<T>>(std::move(processorFunction(*input)));
        }
    };
}

#endif //PIPEX_PROCESSOR_H