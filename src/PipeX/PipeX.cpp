//
// Created by Matteo Ranzi on 24/12/25.
//

#include "PipeX/PipeXEngine.h"
#include "PipeX/nodes/thread_safe/Console_threadsafe.h"

namespace PipeX {
    // Definition of the singleton instance used in header files to prevent multiple definition over different translation units
    PipeXEngine* PipeXEngine::pipex_engine_ = nullptr; // Singleton instance

    // Definition of the mutex instance used in header files to prevent multiple definition over different translation units
    pthread_mutex_t Console_threadsafe::console_mutex = PTHREAD_MUTEX_INITIALIZER;
} // PipeX

//------------------------------------------------
// THIS SECTION IS A COLLECTION OF GENERAL TODOS AND REQUIRED FIXES FOR THE PROJECT
//------------------------------------------------
// TODO Add proper include directives in each source/header file to ensure all dependencies are met.
// TODO Implement unit tests for all classes and methods to ensure correctness and robustness.


// FIXME update all logLifeCycle prints with the proper values