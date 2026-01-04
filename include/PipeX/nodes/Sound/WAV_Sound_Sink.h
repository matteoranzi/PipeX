//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_WAV_SOUND_SINK_H
#define PIPEX_WAV_SOUND_SINK_H

#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

#include "PipeX/metadata/WAV_Metadata.h"
#include "PipeX/nodes/primitives/Sink.h"
#include "PipeX/utils/sound_utils.h"

namespace PipeX {
    /**
     * @brief Sink node that saves audio data to WAV files.
     *
     * Writes each received audio buffer to a separate WAV file with an index suffix and adds the .wav extension.
     */
    class WAV_Sound_Sink final : public Sink<WAV_AudioBuffer, WAV_Metadata> {
    public:
        WAV_Sound_Sink(std::string node_name, std::string filename)
                : Sink(std::move(node_name), [this](std::vector<WAV_AudioBuffer>& audios) {
                    int index = 0;
                    for (auto& audio : audios) {
                        saveWAV2File(audio, filename_ + "_" + std::to_string(index++) + ".wav");
                    }
                }), filename_(std::move(filename)) {
            this->logLifeCycle("Constructor(filename, name)");
        }

    private:
        const std::string filename_;

        void saveWAV2File(const WAV_AudioBuffer& audio, const std::string& filename) const {
            const auto& metadata = this->getMetadata();
            std::ofstream file(filename, std::ios::binary);

            // RIFF
            file.write("RIFF", 4);
            file.write(reinterpret_cast<const char*>(&(metadata->riffSize)), 4);
            file.write("WAVE", 4);

            // fmt chunk
            file.write("fmt ", 4);
            uint32_t fmtSize = 16;
            uint16_t audioFormat = 1; // PCM
            file.write(reinterpret_cast<const char*>(&fmtSize), 4);
            file.write(reinterpret_cast<const char*>(&audioFormat), 2);
            file.write(reinterpret_cast<const char*>(&(metadata->numChannels)), 2);
            file.write(reinterpret_cast<const char*>(&(metadata->sampleRate)), 4);
            file.write(reinterpret_cast<const char*>(&(metadata->byteRate)), 4);
            file.write(reinterpret_cast<const char*>(&(metadata->blockAlign)), 2);
            file.write(reinterpret_cast<const char*>(&(metadata->bitsPerSample)), 2);

            // data chunk
            file.write("data", 4);
            file.write(reinterpret_cast<const char*>(&(metadata->dataSize)), 4);

            // write samples - convert to the correct bit depth
            for (const auto& sample : audio) {
                if (metadata->bitsPerSample == 16) {
                    int16_t sample16 = static_cast<int16_t>(sample);
                    file.write(reinterpret_cast<const char*>(&sample16), sizeof(int16_t));
                } else if (metadata->bitsPerSample == 32) {
                    file.write(reinterpret_cast<const char*>(&sample), sizeof(int32_t));
                } else {
                    // Handle other bit depths as needed
                    file.write(reinterpret_cast<const char*>(&sample), metadata->bytesPerSample);
                }
            }
        }
    };
}

#endif //PIPEX_WAV_SOUND_SINK_H
