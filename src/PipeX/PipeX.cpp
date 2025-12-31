//
// Created by Matteo Ranzi on 24/12/25.
//

#include "PipeX/PipeXEngine.h"
#include "my_extended_cpp_standard/my_memory.h"

namespace PipeX {
    PipeXEngine* PipeXEngine::pipex_engine_ = nullptr; // Singleton instance
    pthread_mutex_t console_mutex = PTHREAD_MUTEX_INITIALIZER;
} // PipeX