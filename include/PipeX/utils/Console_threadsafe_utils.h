//
// Created by Matteo Ranzi on 31/12/25.
//

#ifndef PIPEX_CONSOLE_THREADSAFE_H
#define PIPEX_CONSOLE_THREADSAFE_H

#include <mutex>


namespace PipeX {
    class Console_threadsafe {
        protected:
            static std::mutex console_mutex;
    };
}

#endif //PIPEX_CONSOLE_THREADSAFE_H