#include <iostream>

#include "debug/print_debug.h"
#include "PipeX/PipeXEngine.h"
#include "PipeX/nodes/primitives/Sink.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/nodes/primitives/Source.h"
#include "PipeX/nodes/thread_safe/ConsoleSink_ts.h"
#include "PipeX/nodes/thread_safe/ConsoleSource_ts.h"

int main() {
    const auto pipex_engine = PipeX::PipeXEngine::getPipexEngine();

    pipex_engine->newPipeline("BasicPipeline")
        .addNode<PipeX::Source<int>>([]() {
            return std::vector<int>{1, 2, 3, 4, 5};
        })
        .addNode<PipeX::Transformer<int, int>>([](const int& data) {
            return data * 2;
        })
        .addNode<PipeX::ConsoleSink_ts<int>>("Console Sink for BasicPipeline");

    try {
        pipex_engine->newPipeline("AnotherBasicPipeline")
            // .addNode<PipeX::Source<int>>("mynode", []() {
            //     return std::vector<int>{10, 20, 30};
            // })
            .addNode<PipeX::ConsoleSource_ts<int>>("Console Source for AnotherBasicPipeline")
            .addNode<PipeX::Transformer<int, int>>("another-node", [](const int& data) {
                return data + 5;
            })/*.removeNodeByName("another-node")*/
            // .addNode<PipeX::Sink<int>>("another-node", [](const std::vector<int>& data) {
            //     for (const auto& item : data) {
            //         std::cout << item << " ";
            //     }
            //     std::cout << std::endl;
            // })
            // .removeNodeByName("another-node")
            .addNode<PipeX::ConsoleSink_ts<int>>("Console Sink for AnotherBasicPipeline");
    } catch (PipeX::InvalidPipelineException& e) {
        PRINT_DEBUG_ERROR("Test exception caught while creating pipeline: %s\n", e.what());
        //New pipeline is not created and PipeXEngine continue with the previous ones
    } catch (PipeX::InvalidOperation& e) {
        PRINT_DEBUG_ERROR("Test exception caught while creating pipeline: %s\n", e.what());
    }


    pipex_engine->start();


    return 0;
}
