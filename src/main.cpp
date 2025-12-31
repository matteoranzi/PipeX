#include <iostream>

#include "PipeX/PipeXEngine.h"
#include "PipeX/nodes/primitives/Sink.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/nodes/primitives/Source.h"

int main() {
    const auto pipex_engine = PipeX::PipeXEngine::getPipexEngine();

    pipex_engine->newPipeline("BasicPipeline")
        .addNode<PipeX::Source<int>>([]() {
            return std::vector<int>{1, 2, 3, 4, 5};
        })
        .addNode<PipeX::Transformer<int, int>>([](const int& data) {
            return data * 2;
        })
        .addNode<PipeX::Sink<int>>([](const std::vector<int>& data) {
            for (const auto& item : data) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        });

    try {
        pipex_engine->newPipeline("AnotherBasicPipeline")
            .addNode<PipeX::Source<int>>("mynode", []() {
                return std::vector<int>{10, 20, 30};
            })
            .addNode<PipeX::Transformer<int, int>>("another-node", [](const int& data) {
                return data + 5;
            }).removeNodeByName("another-node")
            .addNode<PipeX::Sink<int>>("another-node", [](const std::vector<int>& data) {
                for (const auto& item : data) {
                    std::cout << item << " ";
                }
                std::cout << std::endl;
            })
            .removeNodeByName("another-node");
    } catch (PipeX::InvalidPipelineException& e) {
        PRINT_DEBUG_ERROR("Test exception caught while creating pipeline: %s\n", e.what());
        //New pipeline is not created and PipeXEngine continue with the previous ones
    }


    pipex_engine->start();


    return 0;
}
