//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_WAV_SOUND_SINK_H
#define PIPEX_WAV_SOUND_SINK_H
#include "PipeX/nodes/primitives/Sink.h"

namespace PipeX {
    using SAMPLE_TYPE = int16_t;
    class WAV_Sound_Sink final : public Sink<SAMPLE_TYPE> {
    public:
        WAV_Sound_Sink();
    };
}

#endif //PIPEX_WAV_SOUND_SINK_H