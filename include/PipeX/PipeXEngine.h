//
// Created by Matteo Ranzi on 24/12/25.
//

#ifndef PIPEX_PIPEX_HPP
#define PIPEX_PIPEX_HPP

#include <vector>

#include "Pipeline.h"
#include "IPipeline.h"
#include "data/Data.h"
#include "data/IData.h"

#include "extended_cpp_standard/memory.h"


//FIXME known limitation: pipelines in PipeXEngine must have source and sink nodes

namespace PipeX {
    class PipeXEngine {
    public:
        PipeXEngine() = default;
        ~PipeXEngine() = default;

        template<typename InputT, typename OutputT>
        Pipeline<InputT, OutputT>& newPipeline(const std::string& name) {
            auto pipeline = make_unique<Pipeline<InputT, OutputT>>(std::move(name));
            pipelines.push_back(std::move(pipeline));
            return *static_cast<Pipeline<InputT, OutputT>*>(pipelines.back().get());
        }

        void start() const {

        }

        void clearPipelines() {
            pipelines.clear();
        }

    private:
        std::vector<std::unique_ptr<IPipeline>> pipelines;
    };
}


#endif //PIPEX_PIPEX_HPP