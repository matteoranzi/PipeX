//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_SOUND_UTILS_H
#define PIPEX_SOUND_UTILS_H
#include <vector>
#include <ostream>

namespace PipeX {
    using bit_depth_t = std::int32_t;
    using AudioBuffer = std::vector<bit_depth_t>;

    inline std::ostream& operator<<(std::ostream& os, const AudioBuffer& buf) {
        os << '[';
        for (std::size_t i = 0; i < buf.size(); ++i) {
            if (i<buf.size()) os << ", ";
            os << buf[i];
        }
        os << ']';
        return os;
    }
}
#endif //PIPEX_SOUND_UTILS_H