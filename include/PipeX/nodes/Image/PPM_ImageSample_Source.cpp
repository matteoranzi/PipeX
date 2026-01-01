//
// Created by Matteo Ranzi on 01/01/26.
//

#include "PipeX/nodes/primitives/Source.h"

namespace PipeX {

    using BIT = std::array<int, 3>; // (RGB triplet)
    using PPM_Image_Data = std::vector<std::vector<BIT>>;

    class PPM_ImageSample_Source final : public Source<PPM_Image_Data> {
        public:
            PPM_ImageSample_Source(const int width, const int height, const int sample, std::string name)
                    : Source<PPM_Image_Data>(std::move(name), [this]() {
                        return this->getImageSample(width_, height_, sample_);
                    }), width_(width), height_(height), sample_(sample) {
                this->logLifeCycle("Constructor(width, height, sample, name)");
            }

    protected:
        std::string typeName() const override {
            return "PPM_ImageSample_Source";
        }

    private:
        const int width_;
        const int height_;
        const int sample_;

        std::vector<PPM_Image_Data> getImageSample(const int width, const int height, const int sample) const {
            switch (sample) {
            case 0:
                return gradientSample(width, height);
            default:
                return {PPM_Image_Data()}; // TODO return blank image
            }
        }

        std::vector<PPM_Image_Data> gradientSample(const int width, const int height) const {
            PPM_Image_Data gradientImage;
            gradientImage.reserve(width * height);
        }
    };
}
