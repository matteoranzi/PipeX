//
// Created by Matteo Ranzi on 24/12/25.
//

#include "PipeX/PipeXEngine.h"
#include "../../include/PipeX/utils/Console_threadsafe_utils.h"
#include <mutex>

namespace PipeX {
    // Definition of the singleton instance used in header files to prevent multiple definition over different translation units
    PipeXEngine* PipeXEngine::pipex_engine_ = nullptr; // Singleton instance

    // Definition of the mutex instance used in header files to prevent multiple definition over different translation units
    std::mutex Console_threadsafe::console_mutex;
} // PipeX