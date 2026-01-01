//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_PPM_IMAGE_SINK_H
#define PIPEX_PPM_IMAGE_SINK_H

#include <fstream>

#include "PipeX/nodes/primitives/Sink.h"

namespace PipeX {
    using BIT = std::array<int, 3>; // (RGB triplet)
    using PPM_Image = std::vector<std::vector<BIT>>;

    class PPM_Image_Sink final: public Sink<PPM_Image> {
    public:
        PPM_Image_Sink(std::string node_name, std::string filename)
                : Sink<PPM_Image>(std::move(node_name), [this](std::vector<PPM_Image>& images) {
                    int index = 0;
                    for (auto& image : images) {
                        saveImage(image, filename_ + "_" + std::to_string(index++) + ".ppm");
                    }
                }), filename_(std::move(filename)) {
            this->logLifeCycle("Constructor(filename, name)");
        }

    private:
        const std::string filename_;

        static void saveImage(const PPM_Image& image, const std::string& filename) {
            std::ofstream ofs(filename);
            if (!ofs) {
                //TODO create a proper exception class
                throw std::runtime_error("Could not open file for writing: " + filename);
            }
            const int height = static_cast<int>(image.size());
            const int width = static_cast<int>(image[120].size());
            ofs << "P3\n" << width << " " << height << "\n255\n";
            for (const auto& row : image) {
                for (const auto& pixel : row) {
                    ofs << pixel[0] << " " << pixel[1] << " " << pixel[2] << "\n";
                }
            }
        }
    };
}

#endif //PIPEX_PPM_IMAGE_SINK_H