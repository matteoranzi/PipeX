//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_WAV_METADATA_HPP
#define PIPEX_WAV_METADATA_HPP

#include <cstdint>

#include "PipeX/metadata/IMetadata.h"

namespace PipeX {
    /**
     * @brief Metadata for WAV audio format.
     *
     * Stores standard WAV header information and derived parameters.
     */
    class WAV_Metadata final : public IMetadata {
    public:

        // WAV parameters
        uint16_t numChannels{};
        uint32_t sampleRate{};
        uint16_t bitsPerSample{};
        uint32_t durationSec{};

        // Derived parameters
        uint16_t bytesPerSample{};
        uint16_t blockAlign{};
        uint32_t byteRate{};
        uint32_t numSamples{};
        uint32_t dataSize{};
        uint32_t riffSize{};

        WAV_Metadata() = default;

        WAV_Metadata(const uint16_t numChannels, const uint32_t sampleRate, const uint16_t bitsPerSample, const uint32_t durationSec) :
                            numChannels(numChannels),
                            sampleRate(sampleRate),
                            bitsPerSample(bitsPerSample),
                            durationSec(durationSec),

                            bytesPerSample(bitsPerSample / 8),
                            blockAlign(numChannels * bytesPerSample),
                            byteRate(sampleRate * blockAlign),
                            numSamples(sampleRate * durationSec),
                            dataSize(numSamples * blockAlign),
                            riffSize(36 + dataSize)
        {}

        ~WAV_Metadata() override = default;

        void setParameters(const uint16_t numChannels_, const uint32_t sampleRate_, const uint16_t bitsPerSample_, const uint32_t durationSec_) {
            this->numChannels = numChannels_;
            this->sampleRate = sampleRate_;
            this->bitsPerSample = bitsPerSample_;
            this->durationSec = durationSec_;

            this->bytesPerSample = bitsPerSample_ / 8;
            this->blockAlign = numChannels_ * bytesPerSample;
            this->byteRate = sampleRate_ * blockAlign;
            this->numSamples = sampleRate_ * durationSec_;
            this->dataSize = numSamples * blockAlign;
            this->riffSize = 36 + dataSize;
        }
    };
}

#endif //PIPEX_WAV_METADATA_HPP