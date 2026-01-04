//
// Created by Matteo Ranzi on 02/01/26.
//

#ifndef PIPEX_EQ_BELLCURVE_HPP
#define PIPEX_EQ_BELLCURVE_HPP

#include <cmath>
#include <string>

#include "PipeX/metadata/WAV_Metadata.h"
#include "PipeX/nodes/primitives/Transformer.h"
#include "PipeX/utils/sound_utils.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace PipeX {
    /**
     * @brief Transformer node that applies a bell curve equalization filter.
     *
     * Implements a peaking EQ filter using a biquad implementation.
     */
    class EQ_BellCurve final : public Transformer<WAV_AudioBuffer, WAV_AudioBuffer, WAV_Metadata> {
    public:
        EQ_BellCurve(std::string node_name, double centerFrequency, double qFactor, double gainDB)
            : Transformer(std::move(node_name), [this, centerFrequency, qFactor, gainDB] (WAV_AudioBuffer& input) {
                return this->applyEQ(input, centerFrequency, qFactor, gainDB);
            }) {
            this->logLifeCycle("EQ_BellCurve(std::string node_name, double centerFrequency, double qFactor, double gainDB)");
        }

    private:
        struct Biquad {
            double b0, b1, b2;
            double a1, a2;
            double x1 = 0, x2 = 0;
            double y1 = 0, y2 = 0;

            double process(double x) {
                double y = b0*x + b1*x1 + b2*x2
                           - a1*y1 - a2*y2;
                x2 = x1;
                x1 = x;
                y2 = y1;
                y1 = y;
                return y;
            }
        };

        WAV_AudioBuffer applyEQ(WAV_AudioBuffer& data, double centerFrequency, double qFactor, double gainDB) const {
            const auto& metadata = this->getMetadata();

            Biquad eq = makePeakingEQ(centerFrequency, qFactor, gainDB, metadata->sampleRate);
            for (auto& sample: data) {
                sample = eq.process(sample);
            }

            return data;
        }


        static Biquad makePeakingEQ(const double f0, const double Q, const double gainDB, const double Fs) {
            Biquad eq;

            const double A = pow(10.0, gainDB / 40.0);
            const double w0 = 2.0 * M_PI * f0 / Fs;
            const double alpha = sin(w0) / (2.0 * Q);

            const double b0 = 1 + alpha * A;
            const double b1 = -2 * cos(w0);
            const double b2 = 1 - alpha * A;
            const double a0 = 1 + alpha / A;
            const double a1 = -2 * cos(w0);
            const double a2 = 1 - alpha / A;

            eq.b0 = b0 / a0;
            eq.b1 = b1 / a0;
            eq.b2 = b2 / a0;
            eq.a1 = a1 / a0;
            eq.a2 = a2 / a0;

            return eq;
        }
    };
}

#endif //PIPEX_EQ_BELLCURVE_HPP
