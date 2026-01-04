#include <iostream>
#include <numeric>
#include <algorithm>
#include <unistd.h>

#include "debug/print_debug.h"
#include "PipeX/PipeXEngine.h"
#include "PipeX/errors/PipeXException.h"
#include "PipeX/errors/InvalidPipelineException.h"
#include "PipeX/errors/InvalidOperation.h"
#include "PipeX/errors/NodeNameConflictException.h"
#include "PipeX/nodes/Image/GainExposure.h"
#include "PipeX/nodes/Image/PPM_ImagePreset_Source.h"
#include "PipeX/nodes/Image/PPM_Image_Sink.h"
#include "PipeX/nodes/primitives/Sink.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/nodes/primitives/Source.h"
#include "PipeX/nodes/Audio/AmplitudeModulation.h"
#include "PipeX/nodes/Audio/EQ_BellCurve.h"
#include "PipeX/nodes/Audio/WAV_AudioPreset_Source.h"
#include "PipeX/nodes/Audio/WAV_Audio_Sink.h"
#include "PipeX/nodes/primitives/Aggregator.h"
#include "PipeX/nodes/primitives/Filter.h"
#include "PipeX/nodes/primitives/Processor.h"
#include "PipeX/nodes/thread_safe/ConsoleSink_ts.h"
#include "PipeX/nodes/thread_safe/ConsoleSource_ts.h"

template <typename T>
void printVector(const std::vector<T>& vec);

void basicPipelineDemo();
void threadsafeConsolePipeXEngineDemo();
void pipelineExceptionsDemo();
void WAV_audioDemo();
void PPM_imageDemo();

//==========================================================================================================

int main() {
    std::cout << "======================" << std::endl;
    std::cout << "PipeX Demo Application" << std::endl;
    std::cout << "======================\n" << std::endl;

    std::cout << "(1) Basic pipeline demo" << std::endl;
    std::cout << "(2) Threa-safe console pipelines demo" << std::endl;
    std::cout << "(3) Pipeline exceptions demo" << std::endl;

    int demo;
    std::cout << "\nSelect demo to run: ";
    std::cin >> demo;

    std::cout << "------------------------------------------------------------------\n" << std::endl;

    switch (demo) {
    case 1:
        basicPipelineDemo();
        break;
    case 2:
        threadsafeConsolePipeXEngineDemo();
        break;
    case 3:
        pipelineExceptionsDemo();
        break;
    case 4:
        WAV_audioDemo();
        break;
    case 5:
        PPM_imageDemo();
        break;

    default:
        std::cout << "Invalid demo selection." << std::endl;
    }

    return 0;
}

//==========================================================================================================

//In this simple demo, PipeXEngine is not used: parallel execution is not demonstrated
// This demo shows how to create a simple pipeline with Source, Transformer, Filter and Sink nodes
// The pipeline takes a vector of integers, multiplies each by 3, filters out odd numbers and collects the results
void basicPipelineDemo() {
    std::vector<int> data_source = {1, 2, 3, 4, 5};
    std::vector<int> data_sink;

    PipeX::Pipeline pipeline("BasicPipelineDemo");

    pipeline.addNode<PipeX::Source<int>>("Source", [data_source]() {
                return data_source;
            })
            .addNode<PipeX::Transformer<int, int>>("Transformer", [](const int& data) {
                return data * 3;
            })
            .addNode<PipeX::Filter<int>>("Filter", [](const int& data) {
                return data % 2 == 0; // Keep even numbers
            })
            .addNode<PipeX::Sink<int>>("Sink", [&data_sink](std::vector<int>& data) {
                data_sink = std::move(data);
            });

    pipeline.run();


    std::cout << "Input data: ";
    printVector<int>(data_source);
    std::cout << "Output data: ";
    printVector<int>(data_sink);
}

//==========================================================================================================

//This demo shows how to create and run two thread-safe console pipelines using PipeXEngine
// The first pipeline generates a vector of integers, multiplies each by 2 and outputs to console
// The second pipeline reads integers from console input, sum up all of them and outputs to console
void threadsafeConsolePipeXEngineDemo() {
    const auto pipexEngine = PipeX::PipeXEngine::getPipexEngine();

    pipexEngine->newPipeline("ConsolePipeline_1")
        .addNode<PipeX::Source<int>>("Source", []() {
            return std::vector<int>{1, 2, 3, 4, 5};
        })
        .addNode<PipeX::Transformer<int, int>>("mul_2", [](const int& data) {
            return data * 2;
        })
        .addNode<PipeX::ConsoleSink_ts<int>>("Console Sink for ConsolePipeline_1");


    pipexEngine->newPipeline("ConsolePipeline_2")
            .addNode<PipeX::ConsoleSource_ts<int>>("Console Source for ConsolePipeline_2")
            .addNode<PipeX::Aggregator<int, int>>("total_sum", [](const std::vector<int>& dataVector) {
                return std::accumulate(dataVector.begin(), dataVector.end(), 0);
            })
            .addNode<PipeX::ConsoleSink_ts<int>>("Console Sink for ConsolePipeline_2");


    pipexEngine->start();
}

//==========================================================================================================

//This demo shows how PipeXEngine handles exceptions during pipeline creation
// The first pipeline is missing a Sink node, which will cause an exception during execution
// The second pipeline has duplicated node names, which will cause an exception during creation. Such corrupted pipeline is removed from PipeXEngine after exception is caught
// After handling the exceptions, a working pipeline is created and executed to show that PipeXEngine continues working after exceptions

void pipelineExceptionsDemo() {
    const auto pipexEngine = PipeX::PipeXEngine::getPipexEngine();

    pipexEngine->newPipeline("NoSink_Pipeline")
            .addNode<PipeX::Source<int>>("Source", []() {
                return std::vector<int>{1, 2, 3, 4, 5};
            })
            .addNode<PipeX::Transformer<int, int>>("square", [](const int& data) {
                return data * data;
            });
    // Note that no Sink node is added, which will cause an exception during execution


    try {
        pipexEngine->newPipeline("DuplicatedNode_Pipeline")
            .addNode<PipeX::Source<int>>("Source", []() {
                return std::vector<int>{1, 2, 3, 4, 5};
            })
            .addNode<PipeX::Transformer<int, int>>("duplicatedName", [](const int& data) {
                return data * 2;
            })
            .addNode<PipeX::Filter<int>>("Filter", [](const int& data) {
                return data % 2 != 0; // Keep odd numbers
            })
            //.removeNodeByName("duplicatedName") // Uncommenting this line would fix the exception
            .addNode<PipeX::Transformer<int, int>>("duplicatedName", [](const int& data) {
                return data -1;
            })
            .addNode<PipeX::Sink<int>>("Sink", [](std::vector<int>& data) {
                // dummy sink placeholder
            });

    } catch (PipeX::NodeNameConflictException& e) {
        // Pipeline is incomplete, so is removed from PipeXEngine
        PRINT_DEBUG_ERROR("[Main] Removing corrupted pipeline \"DuplicatedNode_Pipeline\" from PipeXEngine\n");
        pipexEngine->removePipeline("DuplicatedNode_Pipeline");
    } catch (... ) {
        PRINT_DEBUG_ERROR("[Main] Unknown exception caught while creating pipeline\n");
    }

    // Basic working pipeline to show that PipeXEngine continue working after exception
    pipexEngine->newPipeline("WorkingPipeline")
            .addNode<PipeX::Source<int>>("Source", []() {
                return std::vector<int>{1, 2, 3, 4, 5};
            })
            .addNode<PipeX::Processor<int, float>>("normalized", [](const std::vector<int>& dataVector) {
                std::vector<float> normalizedData;
                normalizedData.reserve(dataVector.size());
                const int maxVal = *std::max_element(dataVector.begin(), dataVector.end());
                for (const auto& val : dataVector) {
                    normalizedData.push_back(static_cast<float>(val) / maxVal);
                }
                return normalizedData;
            })
            .addNode<PipeX::ConsoleSink_ts<float>>("Console Sink for WorkingPipeline");


    pipexEngine->start();

}

//==========================================================================================================

void WAV_audioDemo() {

}

//==========================================================================================================

void PPM_imageDemo() {

}

//==========================================================================================================

/*{
    const auto pipex_engine = PipeX::PipeXEngine::getPipexEngine();
    pipex_engine->newPipeline("PPM Image generation")
        .addNode<PipeX::PPM_ImagePreset_Source>("PPM Image Sample Source", 1024, 1024, 0, 1)
        .addNode<PipeX::GainExposure>("Gain Exposure", 1.3, 10)
        .addNode<PipeX::PPM_Image_Sink>("PPM Image Sink", "output/image/gain_contrast_gradient_square");

    pipex_engine->newPipeline("WAV Audio generation")
            .addNode<PipeX::WAV_SoundPreset_Source>("WAV Audio Sample Source", 1, 44100, 16, 5, 2)
            .addNode<PipeX::EQ_BellCurve>("EQ Bell Curve", 450.0, 6.0, 25.0)
            .addNode<PipeX::WAV_Sound_Sink>("WAV Audio Sink", "output/audio/eq3_pink_noise");


    try {
        pipex_engine->newPipeline("WAV Audio generation with Amplitude Modulation")
            .addNode<PipeX::WAV_SoundPreset_Source>("WAV Audio Sample Source", 1, 44100, 16, 5, 2)
            .addNode<PipeX::EQ_BellCurve>("EQ Bell Curve", 440.0, 4.0, 30.0)
            .addNode<PipeX::EQ_BellCurve>("EQ Bell Curve 2", 831, 4.0, 20.0)
            .addNode<PipeX::AmplitudeModulation>("Amplitude Modulation", 27.0, 0.8)
            .addNode<PipeX::AmplitudeModulation>("Amplitude Modulation 2", 5.0, 0.5);
            // .addNode<PipeX::WAV_Sound_Sink>("WAV Audio Sink", "output/audio/eq2_tremolo_pink_noise_3");
            // .addNode<PipeX::WAV_Sound_Sink>("WAV Audio Sink", "output/audio/test");
    } catch (PipeX::PipeXException& e) {
        PRINT_DEBUG_ERROR("[Main] Exception caught while creating pipeline: %s\n", e.what());
        //New pipeline is not created and PipeXEngine continue with the previous ones
    } catch (...) {
        PRINT_DEBUG_ERROR("[Main] Unknown exception caught while creating pipeline\n");
    }

    try {
        pipex_engine->start();
    } catch (PipeX::PipeXException& e) {
        PRINT_DEBUG_ERROR("[Main] Exception caught while starting pipex engine: %s\n", e.what());
    }

    return 0;
}*/

template <typename T>
void printVector(const std::vector<T>& vec) {
    std::cout << "[";
    for (const auto& element : vec) {
        std::cout << element;
        if (&element != &vec.back()) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}
