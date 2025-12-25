#include <iostream>
#include "PipeX/PipeXEngine.h"
#include "PipeX/nodes/Transformer.h"

int main() {
    PipeX::PipeXEngine pipex_engine;
    pipex_engine.newPipeline<int, int>("BasicPipeline")
        .addNode<PipeX::Transformer<int, int>>([](const int& data) {
            return data * 2;
        });



    return 0;
}
