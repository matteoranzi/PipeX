//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_GENERIC_BASE_DATA_H
#define PIPEX_GENERIC_BASE_DATA_H

#include <memory>

#include "PipeX/metadata/IMetadata.h"

namespace PipeX {
    class IData {
    public:
        virtual ~IData() = default;

        // Optional metadata associated with the data
        std::shared_ptr<IMetadata> metadata;
    };
}

#endif // PIPEX_GENERIC_BASE_DATA_H