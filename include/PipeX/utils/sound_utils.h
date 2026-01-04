//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_SOUND_UTILS_H
#define PIPEX_SOUND_UTILS_H
#include <vector>
#include <ostream>
#include <cstdint>

namespace PipeX {
    /**
     * @brief Type used for audio sample data (32-bit integer).
     */
    using bit_depth_t = std::int32_t;

    /**
     * @brief Represents a buffer of audio samples.
     */
    using WAV_AudioBuffer = std::vector<bit_depth_t>;

    /**
     * @brief Overload operator<< for printing WAV_AudioBuffer.
     *
     * Prints the audio buffer contents to an output stream.
     *
     * @param os The output stream.
     * @param buf The audio buffer to print.
     * @return Reference to the output stream.
     */
    inline std::ostream& operator<<(std::ostream& os, const WAV_AudioBuffer& buf) {
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