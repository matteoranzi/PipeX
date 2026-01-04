//
// Created by Matteo Ranzi on 01/01/26.
//

#ifndef PIPEX_METADATA_H
#define PIPEX_METADATA_H

namespace PipeX {
    /**
     * @brief Interface for metadata associated with data objects.
     *
     * IMetadata serves as a polymorphic base class for all metadata types.
     * It allows attaching additional information to data flowing through the pipeline.
     */
    class IMetadata {
    public:
        virtual ~IMetadata() = default;
    };
}

#endif //PIPEX_METADATA_H