//
// Created by Matteo Ranzi on 02/01/26.
//

#ifndef PIPEX_GAINEXPOSURE_H
#define PIPEX_GAINEXPOSURE_H

#include <cmath>
#include <string>

#include "PipeX/metadata/PPM_Metadata.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/utils/image_utils.h"

namespace PipeX {
    /**
     * @brief Transformer node that adjusts exposure and contrast of PPM images.
     *
     * Applies a sigmoid-based exposure and contrast adjustment to each pixel channel.
     */
    class GainExposure final : public Transformer<PPM_Image, PPM_Image, PPM_Metadata> {
        public:
        GainExposure(std::string node_name, double gain, double contrast = 1.0)
            : Transformer(std::move(node_name), [this, gain, contrast] (PPM_Image& input) {
                return this->grayscale(input, gain, contrast);
            }) {
            this->logLifeCycle("Gain Exposure");
        }

    private:
        PPM_Image grayscale(PPM_Image& data, const double gain, const double contrast) const {
            static const auto& metadata = this->getMetadata();

            for (auto& row: data) {
                for (auto& pixel: row) {
                    for (auto& channel: pixel) {
                        channel = normalizeExposureWithSigmoid(channel, gain, contrast, metadata->bit_depth);
                    }
                }
            }
            return data;
        }

        static int normalizeExposureWithSigmoid(const int value, const double exposure, const double contrast, const int max_value) {
            double multiplier = pow(2.0, exposure);

            double normalized = value / static_cast<double>(max_value);
            double exposed = normalized * multiplier;

            // Sigmoid with adjustable contrast: 1 / (1 + exp(-contrast * (x - 0.5)))
            double shifted = exposed - 0.5;
            double sigmoid = 1.0 / (1.0 + exp(-contrast * shifted));

            return static_cast<uint8_t>(sigmoid * max_value);
        }

    };
}

#endif //PIPEX_GAINEXPOSURE_H