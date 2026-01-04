//
// Created by Matteo Ranzi on 01/01/26.
//

#include "PipeX/nodes/Sound/WAV_SoundPreset_Source.h"
#include "PipeX/errors/PipeXException.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace PipeX {

    WAV_AudioBuffer WAV_SoundPreset_Source::sinusoidalWave() const{
        WAV_AudioBuffer audio;
        audio.resize(this->sourceMetadata->numSamples);
        for (bit_depth_t i = 0; i < this->sourceMetadata->numSamples; ++i) {
            constexpr double frequency = 440.0; // A4 note
            const double t = static_cast<double>(i) / this->sourceMetadata->sampleRate;
            audio[i] = static_cast<bit_depth_t>(32767 * sin(2 * M_PI * frequency * t));
        }

        return audio;
    }

    WAV_AudioBuffer WAV_SoundPreset_Source::whiteNoise() const {
        WAV_AudioBuffer audio;
        audio.resize(this->sourceMetadata->numSamples);
        for (bit_depth_t i = 0; i < this->sourceMetadata->numSamples; ++i) {
            double u = (rand() + 1.0) / (RAND_MAX + 1.0);
            double v = (rand() + 1.0) / (RAND_MAX + 1.0);
            double gaussian = sqrt(-2 * log(u)) * cos(2 * M_PI * v);
            int16_t sample = (int16_t)(gaussian * 10000);

            audio[i] = static_cast<bit_depth_t>(sample);
        }

        return audio;
    }

    WAV_AudioBuffer WAV_SoundPreset_Source::pinkNoise() const {
        WAV_AudioBuffer audio;
        audio.resize(this->sourceMetadata->numSamples);
        std::srand((unsigned)time(nullptr));

        const int NUM_ROWS = 16;
        int rows[NUM_ROWS] = {0};
        int runningSum = 0;
        unsigned long counter = 0;

        for (uint32_t i = 0; i < this->sourceMetadata->numSamples; ++i) {
            int index = 0;
            unsigned long c = ++counter;

            while ((c & 1) == 0) {
                c >>= 1;
                index++;
            }

            if (index < NUM_ROWS) {
                runningSum -= rows[index];
                rows[index] = (rand() % 65536) - 32768;
                runningSum += rows[index];
            }

            int16_t sample = runningSum / NUM_ROWS;

            audio[i] = static_cast<bit_depth_t>(sample);
        }

        return audio;
    }

    WAV_AudioBuffer WAV_SoundPreset_Source::loadWAVFile(const int sample) const{
        throw PipeXException("WAV_SoundPreset_Source::loadWAVFile NOT IMPLEMENTED");
        return {};
    }
}
