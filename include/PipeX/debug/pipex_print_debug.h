//
// Created by Matteo Ranzi on 05/11/25.
//

#ifndef PIPEX_PIPEX_PRINT_DEBUG_H
#define PIPEX_PIPEX_PRINT_DEBUG_H

#ifdef PIPEX_PRINT_DEBUG_ENABLED

#include "debug/print_debug.h"

/**
 * @brief Macro for printing debug information.
 * Only active if PIPEX_PRINT_DEBUG_ENABLED is defined.
 */
#define PIPEX_PRINT_DEBUG_INFO(format, ...) PRINT_DEBUG_INFO("[PipeX] " format, ##__VA_ARGS__)

/**
 * @brief Macro for printing debug warnings.
 * Only active if PIPEX_PRINT_DEBUG_ENABLED is defined.
 */
#define PIPEX_PRINT_DEBUG_WARN(format, ...) PRINT_DEBUG_WARN("[PipeX] " format, ##__VA_ARGS__)

/**
 * @brief Macro for printing debug errors.
 * Only active if PIPEX_PRINT_DEBUG_ENABLED is defined.
 */
#define PIPEX_PRINT_DEBUG_ERROR(format, ...) PRINT_DEBUG_ERROR("[PipeX] " format, ##__VA_ARGS__)

#else

#define PIPEX_PRINT_DEBUG_INFO(format, ...)
#define PIPEX_PRINT_DEBUG_WARN(format, ...)
#define PIPEX_PRINT_DEBUG_ERROR(format, ...)

#endif

#endif //PIPEX_PIPEX_PRINT_DEBUG_H