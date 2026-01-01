//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_WAV_SOUNDSAMPLE_SOURCE_H
#define PIPEX_WAV_SOUNDSAMPLE_SOURCE_H
#include "PipeX/nodes/primitives/Source.h"

namespace PipeX {
    using SAMPLE_TYPE = int16_t;
    class WAV_SoundPreset_Source final : public Source<SAMPLE_TYPE> {
    public:
        WAV_SoundPreset_Source(): Source<SAMPLE_TYPE>("WAV_SoundPreset_Source", []() {
            //TODO implement sound generation presets
            return std::vector<SAMPLE_TYPE>{};
        }) {
            this->logLifeCycle("Constructor()");
        }

    private:

    };
}


#endif //PIPEX_WAV_SOUNDSAMPLE_SOURCE_H