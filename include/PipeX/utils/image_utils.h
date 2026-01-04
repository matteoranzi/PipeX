//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_IMAGE_UTILS_HPP
#define PIPEX_IMAGE_UTILS_HPP

#include <array>
#include <vector>

namespace PipeX {
    /**
     * @brief Represents a single pixel with RGB channels.
     */
    using channels = std::array<int, 3>; // (RGB triplet)

    /**
     * @brief Represents a PPM image as a 2D grid of pixels.
     */
    using PPM_Image = std::vector<std::vector<channels>>;
}

#endif //PIPEX_IMAGE_UTILS_HPP