//
// Created by Matteo Ranzi on 31/12/25.
//

#ifndef PIPEX_CONSOLE_THREADSAFE_H
#define PIPEX_CONSOLE_THREADSAFE_H

#include <pthread.h>

namespace PipeX {
    extern pthread_mutex_t console_mutex;
}

#endif //PIPEX_CONSOLE_THREADSAFE_H