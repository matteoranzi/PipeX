//
// Created by Matteo Ranzi on 04/01/26.
//

#ifndef PIPEX_COLOR2BLACKWHITE_H
#define PIPEX_COLOR2BLACKWHITE_H

#include <cmath>
#include <string>

#include "PipeX/metadata/PPM_Metadata.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/utils/image_utils.h"

namespace PipeX {
    class Color2BlackWhite final : public Transformer<PPM_Image, PPM_Image, PPM_Metadata> {
    public:
        explicit Color2BlackWhite(std::string node_name)
            : Transformer(std::move(node_name), [this] (PPM_Image& input) {
                return this->grayscale(input);
            }) {
            this->logLifeCycle("Color2BlackWhite");
        }

    private:
        PPM_Image grayscale(PPM_Image& data) {
            static const auto& metadata = this->getMetadata();

            for (auto& row: data) {
                for (auto& pixel: row) {
                    auto gray = toGrayscale(pixel);
                    for (auto& channel: pixel) {
                        channel = gray;
                    }
                }
            }
            return data;
        }

        double toGrayscale(channelsT channels) const {
            // Using luminosity method for better grayscale conversion
            double gray = 0.21 * channels[0] + 0.72 * channels[1] + 0.07 * channels[2];
            return static_cast<int>(std::round(gray));
        }
    };
}
#endif //PIPEX_COLOR2BLACKWHITE_H