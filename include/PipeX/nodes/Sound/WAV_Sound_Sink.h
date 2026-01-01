//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_WAV_SOUND_SINK_H
#define PIPEX_WAV_SOUND_SINK_H

#include "PipeX/nodes/primitives/Sink.h"
#include "PipeX/utils/sound_utils.h"

namespace PipeX {
    class WAV_Sound_Sink final : public Sink<AudioBuffer> {
    public:
        WAV_Sound_Sink();
    };
}

#endif //PIPEX_WAV_SOUND_SINK_H