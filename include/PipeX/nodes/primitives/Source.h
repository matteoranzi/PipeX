//
// Created by Matteo Ranzi on 25/12/25.
//

#ifndef PIPEX_SOURCE_H
#define PIPEX_SOURCE_H

#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <utility>

#include "NodeCRTP.h"

namespace PipeX {
    /**
     * @brief A specialized INode that produces data.
     *
     * The Source node is the starting point of a pipeline. It generates data using a
     * user-defined function and passes it to the next node.
     *
     * @tparam T The type of data produced.
     * @tparam MetadataT The type of metadata associated with the data.
     */
    template <typename T, typename MetadataT = IMetadata>
    class Source : public NodeCRTP<Source<T, MetadataT>, T, T, MetadataT> {

        using Base = NodeCRTP<Source, T, T, MetadataT>;
        friend Base;

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
            this->logLifeCycle("MoveConstructor(Source&&)");
        }

        /**
         * @brief Destructor.
         */
        ~Source() override {
            this->logLifeCycle("Destructor()");
        }

        bool isSource() const override final { return true; }

    protected:
        /// Metadata associated with the source data
        std::shared_ptr<MetadataT> sourceMetadata;

        /**
         * @brief Creates and initializes metadata for the source.
         *
         * This method allocates and initializes the metadata object that will be
         * associated with the data produced by this source.
         */
        void createMetadata() {
            this->logLifeCycle("createTypedMetadata()");
            this->sourceMetadata = std::make_shared<MetadataT>();
        }

        /**
         * @brief Returns the type name of this node.
         * @return A string "Source" identifying this node type
         */
        std::string typeName() const override {
            return "Source";
        }


    private:
        /// The source function that generates the initial data
        Function sourceFunction;

        /**
         * @brief Processes (generates) data for the pipeline.
         *
         * For Source nodes, the input parameter is ignored as they generate data
         * from the sourceFunction rather than transforming existing data.
         *
         * @param input Ignored for Source nodes
         * @return A unique pointer to a vector of generated data
         */
        std::unique_ptr<std::vector<T>> processImpl(std::unique_ptr<std::vector<T>>&& input) const override {
            this->logLifeCycle("processImpl(std::unique_ptr<std::vector<InputT>>&&)");

            return extended_std::make_unique<std::vector<T>>(std::move(sourceFunction()));
        }

        /**
         * @brief Post-processing hook that attaches metadata to the output.
         *
         * This method is called after data generation to associate the source
         * metadata with the output data.
         */
        void postProcessHook() const override {
            this->logLifeCycle("afterProcessHook()");
            this->outputData->metadata = this->sourceMetadata;
        }
    };
}

#endif //PIPEX_SOURCE_H
