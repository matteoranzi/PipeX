//
// Created by Matteo Ranzi on 01/01/26.
//


#ifndef PIPEX_NODES_IMAGE_PPM_IMAGESAMPLE_SOURCE_H
#define PIPEX_NODES_IMAGE_PPM_IMAGESAMPLE_SOURCE_H

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "PipeX/metadata/PPM_Metadata.h"
#include "PipeX/nodes/primitives/Source.h"
#include "PipeX/utils/image_utils.h"

namespace PipeX {

    class PPM_ImagePreset_Source final : public Source<PPM_Image, PPM_Metadata> {
        public:
            PPM_ImagePreset_Source(std::string node_name, const int width, const int height, const int preset, const int count)
                    : Source(std::move(node_name), [this]() {
                        this->createMetadata();
                        this->setupPPMMetadata();

                        auto images = std::vector<PPM_Image>();
                        for (int i = 0; i < count_; ++i) {
                            images.push_back(getImagePreset(width_, height_, preset_));
                            auto& image = images.back();

                            if (image.empty() || image[0].empty()) {
                                this->logLifeCycle("Error: Generated image is empty.");
                                //TODO create a proper exception class
                                throw std::runtime_error("Image is empty.");
                            }
                        }
                        return images;
                    }), width_(width), height_(height), count_(count), preset_(preset) {
                this->logLifeCycle("Constructor(width, height, sample, name)");
            }

    protected:
        std::string typeName() const override {
            return "PPM_ImageSample_Source";
        }

    private:
        const int width_;
        const int height_;
        const int count_;
        const int preset_;

        void setupPPMMetadata() const {
            sourceMetadata->width = width_;
            sourceMetadata->height = height_;
            sourceMetadata->bit_depth = 255; // 8 bits per channel
        }

        PPM_Image getImagePreset(const int width, const int height, const int preset) const {
            switch (preset) {
            case 0:
                return gradientImage(width, height);
            case 1:
                return checkerboardImage(width, height);
            case 2:
                return colorCheckImage(width, height);
            default:
                return loadImageFile(preset);
            }
        }

        PPM_Image gradientImage(int width, int height) const;
        PPM_Image checkerboardImage(int width, int height) const;
        PPM_Image colorCheckImage(int width, int height) const;
        PPM_Image loadImageFile(int sample) const;
    };
}

#endif