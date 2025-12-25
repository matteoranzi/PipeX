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
    class PipeXEngine {
    public:
        /**
         * @brief Default constructor.
         */
        PipeXEngine() = default;

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
            pipelines.emplace_back(name);
            return pipelines.back();
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
        //TODO flags to manage the state of the engine (running, ready, error, end, etc.) and of each pipeline

        /// Container holding all registered pipelines
        std::vector<Pipeline> pipelines;

        /**
         * @brief Executes a single pipeline.
         * @param pipeline The pipeline to execute.
         *
         * This static method is used as the entry point for pipeline execution threads.
         * Catches and suppresses all exceptions thrown during pipeline execution.
         */
        static void runPipeline(const Pipeline& pipeline) {
            try {
                pipeline.run();
            } catch (TypeMismatchException &e) {
                PIPEX_PRINT_DEBUG_ERROR("[PipeXEngine] TypeMismatchException exception in pipeline \"%s\": %s\n", pipeline.getName().c_str(), e.what());
            } catch (...) {
                // TODO handle exceptions properly
                PIPEX_PRINT_DEBUG_ERROR("[PipeXEngine] Unknown exception in pipeline: %s\n", pipeline.getName().c_str());
            }
        }
    };
}


#endif //PIPEX_PIPEX_HPP