//
// Created by Matteo Ranzi on 20/12/25.
//

#ifndef PIPEX_GENERIC_BASE_DATA_H
#define PIPEX_GENERIC_BASE_DATA_H

class GenericData {
public:
    virtual ~GenericData() = default;
    virtual std::unique_ptr<GenericData> clone() const = 0;
};

#endif // PIPEX_GENERIC_BASE_DATA_H