#include <iostream>
#include "PipeX/PipeXEngine.h"
#include "PipeX/nodes/Transformer.h"
#include "PipeX/nodes/Source.h"

int main() {
    PipeX::PipeXEngine pipex_engine;
    pipex_engine.newPipeline<int, int>("BasicPipeline")
        .addNode<PipeX::Source<int>>([]() {
            return std::vector<int>{1, 2, 3, 4, 5};
        })
        .addNode<PipeX::Transformer<int, int>>([](const int& data) {
            return data * 2;
        })
        .addNode<PipeX::Sink<int>>([](std::vector<int>& data) {
            for (const auto& item : data) {
                std::cout << item << " ";
            }
            std::cout << std::endl;
        });

    pipex_engine.start();



    return 0;
}
