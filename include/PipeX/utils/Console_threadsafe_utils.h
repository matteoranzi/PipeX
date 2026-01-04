//
// Created by Matteo Ranzi on 31/12/25.
//

#ifndef PIPEX_CONSOLE_THREADSAFE_H
#define PIPEX_CONSOLE_THREADSAFE_H

#include <mutex>

namespace PipeX {
    /**
     * @brief Helper class for thread-safe console access.
     *
     * Provides a static mutex to synchronize access to the console (std::cout/std::cin)
     * across multiple threads. Classes needing thread-safe console I/O should inherit from this.
     */
    class Console_threadsafe {
        protected:
            static std::mutex console_mutex;
    };
}

#endif //PIPEX_CONSOLE_THREADSAFE_H