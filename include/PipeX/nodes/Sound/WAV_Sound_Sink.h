//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_WAV_SOUND_SINK_H
#define PIPEX_WAV_SOUND_SINK_H

#include <fstream>

#include "PipeX/metadata/WAV_Metadata.h"
#include "PipeX/nodes/primitives/Sink.h"
#include "PipeX/utils/sound_utils.h"

namespace PipeX {
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
            file.write(reinterpret_cast<char*>(&(metadata->riffSize)), 4);
            file.write("WAVE", 4);

            // fmt chunk
            file.write("fmt ", 4);
            uint32_t fmtSize = 16;
            uint16_t audioFormat = 1; // PCM
            file.write(reinterpret_cast<char*>(&fmtSize), 4);
            file.write(reinterpret_cast<char*>(&audioFormat), 2);
            uint16_t numChannels = 1;
            file.write(reinterpret_cast<char*>(&(numChannels)), 2);
            // file.write(reinterpret_cast<char*>(&(metadata->numChannels)), 2);
            file.write(reinterpret_cast<char*>(&(metadata->sampleRate)), 4);
            file.write(reinterpret_cast<char*>(&(metadata->byteRate)), 4);
            file.write(reinterpret_cast<char*>(&(metadata->blockAlign)), 2);
            file.write(reinterpret_cast<char*>(&(metadata->bitsPerSample)), 2);

            // data chunk
            file.write("data", 4);
            file.write(reinterpret_cast<char*>(&(metadata->dataSize)), 4);

            // write samples
            for (const auto& sample : audio) {
                file.write(reinterpret_cast<const char*>(&sample), sizeof(bit_depth_t));
            }
        }
    };
}

#endif //PIPEX_WAV_SOUND_SINK_H