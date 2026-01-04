//
// Created by Matteo Ranzi on 02/01/26.
//

#ifndef PIPEX_GAINAUDIO_HPP
#define PIPEX_GAINAUDIO_HPP

#include <cmath>
#include <string>

#include "PipeX/metadata/WAV_Metadata.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/utils/sound_utils.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace PipeX {
    class AmplitudeModulation final : public Transformer<WAV_AudioBuffer, WAV_AudioBuffer, WAV_Metadata> {
    public:
        AmplitudeModulation(std::string node_name, double rateHz, double depth)
            : Transformer(std::move(node_name), [this, rateHz, depth] (WAV_AudioBuffer& input) {
                return this->applyAmplitudeModulation(input, rateHz, depth);
            }) {
            this->logLifeCycle("AmplitudeModulation(std::string node_name, double modulationIndex, double modulationFrequency)");
        }

    private:
        WAV_AudioBuffer applyAmplitudeModulation(WAV_AudioBuffer& data, double rateHz, double depth) const {
            const auto& metadata = this->getMetadata();
            const double sampleRate = static_cast<double>(metadata->sampleRate);
            WAV_AudioBuffer modulatedData(data.size());

            for (std::size_t n = 0; n < data.size(); ++n) {
                double modulation = (1.0 + depth * sin(2.0 * M_PI * rateHz * n / sampleRate)) / 2.0;
                modulatedData[n] = static_cast<bit_depth_t>(data[n] * modulation);
            }

            return modulatedData;
        }
    };
}

#endif //PIPEX_GAINAUDIO_HPP
