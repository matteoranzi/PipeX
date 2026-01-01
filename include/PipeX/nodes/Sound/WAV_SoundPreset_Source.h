//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_WAV_SOUNDSAMPLE_SOURCE_H
#define PIPEX_WAV_SOUNDSAMPLE_SOURCE_H

#include "PipeX/metadata/WAV_Metadata.h"
#include "PipeX/nodes/primitives/Source.h"
#include "PipeX/utils/sound_utils.h"

namespace PipeX {
    class WAV_SoundPreset_Source final : public Source<AudioBuffer, WAV_Metadata> {
    public:
        WAV_SoundPreset_Source(std::string node_name, const int numChannels, const int sampleRate, const int bitsPerSample, const int durationSec, const int preset = 0)
        : Source(std::move(node_name), [this]() {
            this->createMetadata();
            this->setupWAVMetadata();

            auto audioTracks = std::vector<AudioBuffer>();
            audioTracks.reserve(this->sourceMetadata->numChannels);
            for (int i = 0; i < this->sourceMetadata->numChannels; i++) {
                audioTracks.push_back(getSoundPreset());
            }

            return audioTracks;
        }), numChannels(numChannels), sampleRate(sampleRate), bitsPerSample(bitsPerSample), durationSec(durationSec), preset(preset) {
            this->logLifeCycle("Constructor(std::string node_name, const int numChannels, const int sampleRate, const int bitsPerSample, const int durationSec)");
        }


    private:
        int numChannels;
        int sampleRate;
        int bitsPerSample;
        int durationSec;

        int preset;

        void setupWAVMetadata() const;

        AudioBuffer getSoundPreset() const {
            switch (preset) {
            case 0:
                return sinusoidalWave();
            case 1:
                return whiteNoise();
            case 2:
                return pinkNoise();
            default:
                return loadWAVFile(preset);
            }
        }

        AudioBuffer sinusoidalWave() const;
        AudioBuffer whiteNoise() const;
        AudioBuffer pinkNoise() const;
        AudioBuffer loadWAVFile(int sample) const;
    };
}


#endif //PIPEX_WAV_SOUNDSAMPLE_SOURCE_H