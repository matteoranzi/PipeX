//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_SOUND_UTILS_H
#define PIPEX_SOUND_UTILS_H
#include <cstdint>
#include <vector>

namespace PipeX {
    using bit_depth_t = std::int32_t;
    using AudioBuffer = std::vector<bit_depth_t>;
}
#endif //PIPEX_SOUND_UTILS_H