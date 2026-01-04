//
// Created by Matteo Ranzi on 09/11/25.
//

#ifndef PIPEX_MEMORY_H
#define PIPEX_MEMORY_H

#include <memory>
#include <utility>

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

#endif //PIPEX_MEMORY_H