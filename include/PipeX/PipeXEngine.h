//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_PIPEX_HPP
#define PIPEX_PIPEX_HPP

#include <iostream>
#include <vector>
#include <thread>

#include "Pipeline.h"

// TODO locking mechanism to avoid modifications while running asynchronously (thread safe)
namespace PipeX {
    /**
     * @class PipeXEngine
     * @brief Main engine class for managing and executing multiple pipelines concurrently.
     *
     * PipeXEngine provides functionality to create, manage, and execute pipelines in parallel.
     * Each pipeline runs in its own thread when the engine is started.
     */
    //TODO Prevent reallocation/modification while running (e.g., mutex + state flags, reserve enough capacity beforehand, or forbid API calls that mutate).
    // TODO implement state management (running, stopped, error, etc.) and methods to query the state of the engine and individual pipelines

    class PipeXEngine {
    private:
        static PipeXEngine* pipex_engine_; // Singleton instance

    public:
        // Singleton instance of the PipeXEngine
        static PipeXEngine* getPipexEngine() {
            if (pipex_engine_ == nullptr) {
                pipex_engine_ = new PipeXEngine();
            }
            return PipeXEngine::pipex_engine_;
        }

        // Delete copy constructor and assignment
        PipeXEngine(const PipeXEngine&) = delete;
        PipeXEngine& operator=(const PipeXEngine&) = delete;

        /**
         * @brief Default destructor.
         */
        ~PipeXEngine() = default;

        /**
         * @brief Creates a new pipeline with the specified name.
         * @param name The name identifier for the pipeline.
         * @return Reference to the newly created pipeline.
         */
        Pipeline& newPipeline(const std::string& name) {
            try {
                pipelines.emplace_back(std::make_shared<Pipeline>(name));
                return *pipelines.back();
            } catch (InvalidPipelineException& e) {
                PIPEX_PRINT_DEBUG_ERROR("[PipeXEngine] InvalidPipelineException exception while creating new pipeline \"%s\": %s\n", name.c_str(), e.what());
                throw;
            }
        }

        /**
         * @brief Starts execution of all pipelines in parallel.
         *
         * Creates a separate thread for each pipeline and executes them concurrently.
         * Blocks until all pipelines have completed execution.
         * Exceptions thrown during pipeline execution are caught but not propagated.
         */
        // TODO implement parallel run via threads
        // TODO implement asynch run via flags
        void start() const {
            std::vector<std::thread> threads;
            threads.reserve(pipelines.size());
            for (auto& pipeline : pipelines) {
                threads.emplace_back(&PipeXEngine::runPipeline, pipeline);
            }

            // Wait all threads to finish
            for (auto& thread : threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }

        /**
         * @brief Removes all pipelines from the engine.
         */
        void clearPipelines() {
            pipelines.clear();
        }

    private:

        /**
         * @brief Private default constructor for singleton pattern.
         */
        PipeXEngine() = default;

        //TODO flags to manage the state of the engine (running, ready, error, end, etc.) and of each pipeline

        /// Container holding all registered pipelines
        std::vector<std::shared_ptr<Pipeline>> pipelines;

        /**
         * @brief Executes a single pipeline.
         * @param pipeline The pipeline to execute.
         *
         * This static method is used as the entry point for pipeline execution threads.
         * Catches and suppresses all exceptions thrown during pipeline execution.
         */
        static void runPipeline(const std::shared_ptr<Pipeline>& pipeline) {
            try {
                pipeline->run();
            } catch (TypeMismatchException &e) {
                PIPEX_PRINT_DEBUG_ERROR("[PipeXEngine] TypeMismatchException exception in pipeline \"%s\": %s\n", pipeline->getName().c_str(), e.what());
            } catch (InvalidPipelineException &e) {
                PIPEX_PRINT_DEBUG_ERROR("[PipeXEngine] InvalidPipelineException exception in pipeline \"%s\": %s\n", pipeline->getName().c_str(), e.what());
            }
            catch (...) {
                PIPEX_PRINT_DEBUG_ERROR("[PipeXEngine] Unknown exception in pipeline: %s\n", pipeline->getName().c_str());
            }
        }

    };

    PipeXEngine* PipeXEngine::pipex_engine_ = nullptr;
}

#endif //PIPEX_PIPEX_HPP