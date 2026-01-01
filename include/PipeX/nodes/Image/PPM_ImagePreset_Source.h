//
// Created by Matteo Ranzi on 01/01/26.
//


#ifndef PIPEX_NODES_IMAGE_PPM_IMAGESAMPLE_SOURCE_H
#define PIPEX_NODES_IMAGE_PPM_IMAGESAMPLE_SOURCE_H

#include <iostream>

#include "PipeX/nodes/primitives/Source.h"

namespace PipeX {

    using BIT = std::array<int, 3>; // (RGB triplet)
    using PPM_Image = std::vector<std::vector<BIT>>;

    class PPM_ImagePreset_Source final : public Source<PPM_Image> {
        public:
            PPM_ImagePreset_Source(std::string node_name, const int width, const int height, const int preset, const int count)
                    : Source<PPM_Image>(std::move(node_name), [this]() {
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
                    }), width_(width), height_(height), preset_(preset), count_(count) {
                this->logLifeCycle("Constructor(width, height, sample, name)");
            }

    protected:
        std::string typeName() const override {
            return "PPM_ImageSample_Source";
        }

    private:
        const int width_;
        const int height_;
        const int preset_;
        const int count_;

        static PPM_Image getImagePreset(const int width, const int height, const int preset) {
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

        static PPM_Image gradientImage(int width, int height);
        static PPM_Image checkerboardImage(int width, int height);
        static PPM_Image colorCheckImage(int width, int height);
        static PPM_Image loadImageFile(int sample);
    };
}

#endif