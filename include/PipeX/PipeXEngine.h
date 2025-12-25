//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_PIPEX_HPP
#define PIPEX_PIPEX_HPP

#include <vector>

#include "Pipeline.h"

namespace PipeX {
    class PipeXEngine {
    public:
        PipeXEngine() = default;
        ~PipeXEngine() = default;

        Pipeline& newPipeline(const std::string& name) {
            pipelines.emplace_back(name);
            return pipelines.back();
        }

        void start() const {
            for (auto& pipeline : pipelines) {
                pipeline.run();
            }
        }

        void clearPipelines() {
            pipelines.clear();
        }

    private:
        std::vector<Pipeline> pipelines;
    };
}


#endif //PIPEX_PIPEX_HPP