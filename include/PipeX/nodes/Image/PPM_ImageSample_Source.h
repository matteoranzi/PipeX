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

    class PPM_ImageSample_Source final : public Source<PPM_Image> {
        public:
            PPM_ImageSample_Source(std::string node_name, const int width, const int height, const int sample, const int count)
                    : Source<PPM_Image>(std::move(node_name), [this]() {
                        auto images = std::vector<PPM_Image>();
                        for (int i = 0; i < count_; ++i) {
                            images.push_back(getImageSample(width_, height_, sample_));
                            auto& image = images.back();
                            if (image.empty() || image[0].empty()) {
                                this->logLifeCycle("Error: Generated image is empty.");
                                //TODO create a proper exception class
                                throw std::runtime_error("Image is empty.");
                            }
                        }
                        return images;
                    }), width_(width), height_(height), sample_(sample), count_(count) {
                this->logLifeCycle("Constructor(width, height, sample, name)");
            }

    protected:
        std::string typeName() const override {
            return "PPM_ImageSample_Source";
        }

    private:
        const int width_;
        const int height_;
        const int sample_;
        const int count_;

        static PPM_Image getImageSample(const int width, const int height, const int sample) {
            switch (sample) {
            case 0:
                return gradientSample(width, height);
            case 1:
                return checkerboardSample(width, height);
            case 2:
                return colorCheckSample(width, height);
            default:
                return loadSampleFile(sample);
            }
        }

        static PPM_Image gradientSample(int width, int height);
        static PPM_Image checkerboardSample(int width, int height);
        static PPM_Image colorCheckSample(int width, int height);
        static PPM_Image loadSampleFile(int sample);
    };
}

#endif