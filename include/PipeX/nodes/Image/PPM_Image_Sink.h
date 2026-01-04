//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_PPM_IMAGE_SINK_H
#define PIPEX_PPM_IMAGE_SINK_H

#include <fstream>
#include <string>
#include <vector>

#include "PipeX/nodes/primitives/Sink.h"
#include "PipeX/utils/image_utils.h"
#include "PipeX/errors/PipeX_IO_Exception.h"

namespace PipeX {
    /**
     * @brief Sink node that saves PPM images to files.
     *
     * Writes each received image to a separate PPM file with an index suffix and adds the .ppm extension.
     */
    class PPM_Image_Sink final: public Sink<PPM_Image, PPM_Metadata> {
    public:
        PPM_Image_Sink(std::string node_name, std::string filename)
                : Sink(std::move(node_name), [this](const std::vector<PPM_Image>& images) {
                    int index = 0;
                    for (auto& image : images) {
                        saveToFile(image, filename_ + "_" + std::to_string(index++) + ".ppm");
                    }
                }), filename_(std::move(filename)) {
            this->logLifeCycle("Constructor(filename, name)");
        }

    private:
        const std::string filename_;

        void saveToFile(const PPM_Image& image, const std::string& filename) const {
            const auto&metadata = this->getMetadata();

            std::ofstream file(filename);
            if (!file) {
                throw PipeX_IO_Exception("[PPM_Image_Sink::saveToFile] Could not open file for writing: " + filename
                    + ", make sure the directory exists.");
            }

            const int height = metadata->height;
            const int width = metadata->width;
            file << "P3\n" << width << " " << height << "\n" << metadata->bit_depth << "\n";
            for (const auto& row : image) {
                for (const auto& pixel : row) {
                    file << pixel[0] << " " << pixel[1] << " " << pixel[2] << "\n";
                }
            }
        }
    };
}

#endif //PIPEX_PPM_IMAGE_SINK_H