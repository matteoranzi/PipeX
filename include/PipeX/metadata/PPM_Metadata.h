//
// Created by Matteo Ranzi on 02/01/26.
//

#ifndef PIPEX_PPM_METADATA_H
#define PIPEX_PPM_METADATA_H

#include "PipeX/metadata/IMetadata.h"

namespace PipeX {
    class PPM_Metadata final : public IMetadata {
    public:
        const int channels = 3;// RGB triplet

        int bit_depth{};

        int width{};
        int height{};

        PPM_Metadata() = default;
        PPM_Metadata(int bit_depth, int width, int height);


    };
}

#endif //PIPEX_PPM_METADATA_H