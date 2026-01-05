#include <iostream>
#include <numeric>
#include <algorithm>

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
#include "PipeX/nodes/Image/Color2BlackWhite.h"
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
    std::cout << "(4) WAV audio demo" << std::endl;
    std::cout << "(5) PPM image demo" << std::endl;

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
    std::cout << "Starting Basic pipeline demo..." << std::endl;

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

    std::cout << "End of Basic pipeline demo." << std::endl;
}

//==========================================================================================================

//This demo shows how to create and run two thread-safe console pipelines using PipeXEngine
// The first pipeline generates a vector of integers, multiplies each by 2 and outputs to console
// The second pipeline reads integers from console input, sum up all of them and outputs to console
void threadsafeConsolePipeXEngineDemo() {
    std::cout << "Starting Threa-safe console pipelines demo..." << std::endl;

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

    std::cout << "End of Thread-safe console pipelines demo." << std::endl;
}

//==========================================================================================================

//This demo shows how PipeXEngine handles exceptions during pipeline creation
// The first pipeline is missing a Sink node, which will cause an exception during execution
// The second pipeline has duplicated node names, which will cause an exception during creation. Such corrupted pipeline is removed from PipeXEngine after exception is caught
// After handling the exceptions, a working pipeline is created and executed to show that PipeXEngine continues working after exceptions
void pipelineExceptionsDemo() {
    std::cout << "Starting Pipeline exceptions demo..." << std::endl;

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
        // Pipeline is incomplete, so it is removed from PipeXEngine
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

    std::cout << "End of Pipeline exceptions demo." << std::endl;
}

//==========================================================================================================

// This demo shows how to create a pipeline that generates WAV audio files with amplitude modulation effects
void WAV_audioDemo() {
    std::cout << "Starting WAV audio demo..." << std::endl;

    const auto pipexEngine = PipeX::PipeXEngine::getPipexEngine();

    // WAV audio generation parameters
    constexpr int nStreams = 1; // number of audio files to generate
    constexpr int sampleRate = 44100; // samples per second
    constexpr int bitsPerSample = 16;
    constexpr int durationSec = 5; // duration of each audio file in seconds
    constexpr int preset = 2; // 0: sinusoidal wave, 1: white noise, 2: pink noise

    // EQ Bell Curve parameters
    constexpr double centerFrequency = 831.0; // Hz
    constexpr double qFactor = 4.0; // Quality factor
    constexpr double gainDB = 30.0; // Gain in decibels

    // Amplitude Modulation parameters
    constexpr double rateHz_1 = 27.0; // Modulation frequency in Hz > 0
    constexpr double depth_1 = 0.7; // Modulation depth (0.0 to 1.0)
    constexpr double rateHz_2 = 5.0; // Modulation frequency in Hz > 0
    constexpr double depth_2 = 0.4; // Modulation depth (0.0 to 1.0)

    pipexEngine->newPipeline("WAV Audio generation with Amplitude Modulation")
            .addNode<PipeX::WAV_SoundPreset_Source>("WAV Audio Sample Source", nStreams, sampleRate, bitsPerSample, durationSec, preset)
            .addNode<PipeX::EQ_BellCurve>("EQ Bell Curve", centerFrequency, qFactor, gainDB)
            .addNode<PipeX::AmplitudeModulation>("Amplitude Modulation 1", rateHz_1, depth_1)
            .addNode<PipeX::AmplitudeModulation>("Amplitude Modulation 2", rateHz_2, depth_2)
            .addNode<PipeX::WAV_Sound_Sink>("WAV Audio Sink", "output/audio/tremolo_pink_noise");

    pipexEngine->start();

    std::cout << "File audio generated in output/audio folder" << std::endl;
    std::cout << "End of WAV audio demo." << std::endl;
}

//==========================================================================================================

void PPM_imageDemo() {
    std::cout << "Starting PPM image demo..." << std::endl;

    const auto pipexEngine = PipeX::PipeXEngine::getPipexEngine();

    // PPM image generation parameters
    constexpr int width = 1024;
    constexpr int height = 1024;
    constexpr int preset = 0; // 0: gradient, 1: checkerboard, 2: color checkerboard || only 0 (gradient) is implemented for now
    constexpr int count = 1; // number of images to generate

    // Gain Exposure parameters
    constexpr double gain = 3.2; // Gain factor
    constexpr double contrast = 0.4; // Contrast factor


    pipexEngine->newPipeline("PPM Image generation")
        .addNode<PipeX::PPM_ImagePreset_Source>("PPM Image Sample Source", width, height, preset, count)
        .addNode<PipeX::GainExposure>("Gain Exposure", gain, contrast)
        // .addNode<PipeX::Color2BlackWhite>("Black and White Converter")
        .addNode<PipeX::PPM_Image_Sink>("PPM Image Sink", "output/image/gradient");

    pipexEngine->start();

    std::cout << "File image generated in output/image folder" << std::endl;
    std::cout << "End of PPM image demo." << std::endl;
}

//==========================================================================================================


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
