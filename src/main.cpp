#include <iostream>

#include "debug/print_debug.h"
#include "PipeX/PipeXEngine.h"
#include "PipeX/nodes/Image/GainExposure.h"
#include "PipeX/nodes/Image/PPM_ImagePreset_Source.h"
#include "PipeX/nodes/Image/PPM_Image_Sink.h"
#include "PipeX/nodes/primitives/Sink.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/nodes/primitives/Source.h"
#include "PipeX/nodes/Sound/AmplitudeModulation.h"
#include "PipeX/nodes/Sound/EQ_BellCurve.h"
#include "PipeX/nodes/Sound/WAV_SoundPreset_Source.h"
#include "PipeX/nodes/Sound/WAV_Sound_Sink.h"
#include "PipeX/nodes/thread_safe/ConsoleSink_ts.h"
#include "PipeX/nodes/thread_safe/ConsoleSource_ts.h"

int main() {
    const auto pipex_engine = PipeX::PipeXEngine::getPipexEngine();
    // pipex_engine->newPipeline("PPM Image generation")
    //     .addNode<PipeX::PPM_ImagePreset_Source>("PPM Image Sample Source", 1024, 1024, 0, 1)
    //     .addNode<PipeX::GainExposure>("Gain Exposure", -1.3, 10)
    //     .addNode<PipeX::PPM_Image_Sink>("PPM Image Sink", "output/image/gain_contrast_gradient_square");
    //
    // pipex_engine->newPipeline("WAV Audio generation")
    //         .addNode<PipeX::WAV_SoundPreset_Source>("WAV Audio Sample Source", 1, 44100, 16, 5, 2)
    //         .addNode<PipeX::EQ_BellCurve>("EQ Bell Curve", 450.0, 6.0, 25.0)
    //         .addNode<PipeX::WAV_Sound_Sink>("WAV Audio Sink", "output/audio/eq3_pink_noise");


    try {
        pipex_engine->newPipeline("WAV Audio generation")
            .addNode<PipeX::WAV_SoundPreset_Source>("WAV Audio Sample Source", 1, 44100, 16, 5, 2)
            .addNode<PipeX::EQ_BellCurve>("EQ Bell Curve", 440.0, 4.0, 30.0)
            .addNode<PipeX::EQ_BellCurve>("EQ Bell Curve 2", 831, 4.0, 20.0)
            .addNode<PipeX::AmplitudeModulation>("Amplitude Modulation", 27.0, 0.8)
            .addNode<PipeX::AmplitudeModulation>("Amplitude Modulation 2", 5.0, 0.5)
            .addNode<PipeX::WAV_Sound_Sink>("WAV Audio Sink", "output/audio/eq2_tremolo_pink_noise_3");
            // .addNode<PipeX::WAV_Sound_Sink>("WAV Audio Sink", "output/audio/test");
    } catch (PipeX::PipeXException& e) {
        PRINT_DEBUG_ERROR("Exception caught while creating pipeline: %s\n", e.what());
        //New pipeline is not created and PipeXEngine continue with the previous ones
    } catch (...) {
        PRINT_DEBUG_ERROR("Unknown exception caught while creating pipeline\n");
    }

    pipex_engine->start();

    return 0;
}

// TODO put this code in a test file
/*{
    const auto pipex_engine = PipeX::PipeXEngine::getPipexEngine();

    pipex_engine->newPipeline("BasicPipeline")
        .addNode<PipeX::Source<int>>("Source", []() {
            return std::vector<int>{1, 2, 3, 4, 5};
        })
        .addNode<PipeX::Transformer<int, int>>("Transformer", [](const int& data) {
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
            })
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
    } catch (... ) {
        PRINT_DEBUG_ERROR("Unknown exception caught while creating pipeline\n");
    }


    try {
        pipex_engine->start();
    } catch (PipeX::InvalidOperation& e) {
        PRINT_DEBUG_WARN("Invalid operation detected: %s\n", e.what());
    }


    return 0;
}*/
