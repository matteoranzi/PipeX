//
// Created by Matteo Ranzi on 01/01/26.
//

#include "PipeX/nodes/Image/PPM_ImageSample_Source.h"

#include <iostream>

PipeX::PPM_Image PipeX::PPM_ImageSample_Source::gradientSample(const int width, const int height) {
    PPM_Image image;
    image.resize(height);
    for (auto& column : image) {
        column.reserve(width);
    }
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            const auto r = static_cast<double>(i) / (width-1);
            const auto g = static_cast<double>(j) / (height-1);
            constexpr auto b = 0.0;

            const int ir = static_cast<int>(255.999 * r);
            const int ig = static_cast<int>(255.999 * g);
            constexpr int ib = static_cast<int>(255.999 * b);

            image[j].emplace_back(BIT{ir, ig, ib});
        }
    }

    return image;
}

PipeX::PPM_Image PipeX::PPM_ImageSample_Source::checkerboardSample(const int width, const int height) {
    return {};
}

PipeX::PPM_Image PipeX::PPM_ImageSample_Source::colorCheckSample(const int width, const int height) {
    return {};
}

PipeX::PPM_Image PipeX::PPM_ImageSample_Source::loadSampleFile(const int sample) {
    return {};
}
