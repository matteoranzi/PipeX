//
// Created by Matteo Ranzi on 09/11/25.
//

#ifndef PIPEX_MEMORY_H
#define PIPEX_MEMORY_H

#include <memory>
#include <utility>

namespace extended_std {
    // Not all compilers proved std::make_unique in C++11, so a custom implementation is provided here
    // The following implementation is the same as the one in C++14 standard
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&& ...args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }
}

#endif //PIPEX_MEMORY_H