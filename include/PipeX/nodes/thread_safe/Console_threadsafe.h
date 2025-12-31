//
// Created by Matteo Ranzi on 31/12/25.
//

#ifndef PIPEX_CONSOLE_THREADSAFE_H
#define PIPEX_CONSOLE_THREADSAFE_H

#include <pthread.h>

namespace PipeX {
    class Console_threadsafe {
        protected:
            static void lockConsole() {
                pthread_mutex_lock(&console_mutex);
            }

            static void unlockConsole() {
                pthread_mutex_unlock(&console_mutex);
            }

        private:
            static pthread_mutex_t console_mutex;
    };
}

#endif //PIPEX_CONSOLE_THREADSAFE_H