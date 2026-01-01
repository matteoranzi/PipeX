//
// Created by Matteo Ranzi on 01/01/26.
//

#include "PipeX/nodes/Sound/WAV_SoundPreset_Source.h"

namespace PipeX {
    void WAV_SoundPreset_Source::setupWAVMetadata() const {
        const auto wavMetadata = getWAV_Metadata();
        wavMetadata->bytesPerSample = wavMetadata->bitsPerSample_ / 8;
        wavMetadata->blockAlign     = wavMetadata->numChannels_ * wavMetadata->bytesPerSample;
        wavMetadata->byteRate       = wavMetadata->sampleRate_ * wavMetadata->blockAlign;
        wavMetadata->numSamples     = wavMetadata->sampleRate_ * wavMetadata->durationSec_;
        wavMetadata->dataSize       = wavMetadata->numSamples * wavMetadata->blockAlign;
        wavMetadata->riffSize       = 36 + wavMetadata->dataSize;
    }


    AudioBuffer WAV_SoundPreset_Source::sinusoidalWave() const{
        setupWAVMetadata();
        const auto wavMetadata = getWAV_Metadata();

        AudioBuffer audio;
        audio.resize(wavMetadata->numSamples);
        for (bit_depth_t i = 0; i < wavMetadata->numSamples; ++i) {
            constexpr double frequency = 440.0; // A4 note
            const double t = static_cast<double>(i) / wavMetadata->sampleRate_;
            audio[i] = static_cast<bit_depth_t>(32767 * sin(2 * M_PI * frequency * t));
        }

        return audio;
    }

    AudioBuffer WAV_SoundPreset_Source::loadWAVFile(const int sample) const{
        throw PipeXException("WAV_SoundPreset_Source::loadWAVFile NOT IMPLEMENTED");
        return {};
    }
}
