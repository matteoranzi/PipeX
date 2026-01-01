//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_WAV_SOUNDSAMPLE_SOURCE_H
#define PIPEX_WAV_SOUNDSAMPLE_SOURCE_H

#include "PipeX/errors/MetadataTypeMismatchException.h"
#include "PipeX/metadata/WAV_Metadata.h"
#include "PipeX/nodes/primitives/Source.h"
#include "PipeX/utils/sound_utils.h"

namespace PipeX {
    class WAV_SoundPreset_Source final : public Source<AudioBuffer> {
    public:
        WAV_SoundPreset_Source(std::string node_name, const int numChannels, const int sampleRate, const int bitsPerSample, const int durationSec)
        : Source(std::move(name), []() {
            auto audioTracks = AudioBuffer();
            for (int i = 0; i < numChannels; i++) {

            }
        }) {
            this->logLifeCycle("Constructor()");
            auto wav_metadata = getWAV_Metadata();
        }


    private:
        std::shared_ptr<WAV_Metadata> getWAV_Metadata() const {
            const auto wav_metadata = std::dynamic_pointer_cast<WAV_Metadata>(this->INode);
            if (!wav_metadata) {
                throw MetadataTypeMismatchException(this->getName(), typeid(WAV_Metadata), typeid(this->metadata.get()));
            }

            return wav_metadata;
        }

        void setupWAVMetadata() const;

        AudioBuffer getSoundPreset(const int preset) {
            switch (preset) {
            case 0:
                return sinusoidalWave();

            default:
                return loadWAVFile(preset);
            }
        }

        AudioBuffer sinusoidalWave() const;
        AudioBuffer loadWAVFile(int sample) const;
    };
}


#endif //PIPEX_WAV_SOUNDSAMPLE_SOURCE_H