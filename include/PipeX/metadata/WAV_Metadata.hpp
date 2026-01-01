//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_WAV_METADATA_HPP
#define PIPEX_WAV_METADATA_HPP

#include <cstdint>

#include "PipeX/metadata/IMetadata.h"

namespace PipeX {
    struct WAV_Metadata final : IMetadata {
        // WAV parameters
        uint16_t numChannels_;
        uint32_t sampleRate_;
        uint16_t bitsPerSample_;
        uint32_t durationSec_;

        // Derived parameters
        uint16_t bytesPerSample;
        uint16_t blockAlign;
        uint32_t byteRate;
        uint32_t numSamples;
        uint32_t dataSize;
        uint32_t riffSize;

        WAV_Metadata(const uint16_t numChannels, const uint32_t sampleRate, const uint16_t bitsPerSample, const uint32_t durationSec) :
                            numChannels_(numChannels),
                            sampleRate_(sampleRate),
                            bitsPerSample_(bitsPerSample),
                            durationSec_(durationSec),

                            bytesPerSample(bitsPerSample / 8),
                            blockAlign(numChannels * bytesPerSample),
                            byteRate(sampleRate * blockAlign),
                            numSamples(sampleRate * durationSec * durationSec),
                            dataSize(numSamples * blockAlign),
                            riffSize(36 + dataSize)
        {}
    };
}

#endif //PIPEX_WAV_METADATA_HPP