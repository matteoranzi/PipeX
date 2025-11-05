//
// Created by Matteo Ranzi on 26/09/25.
//

#ifndef PRINT_DEBUG_H
#define PRINT_DEBUG_H

#ifdef __cplusplus
    #include <cstdio>
#else
    #include <stdio.h>
#endif

/*
===========================================================
 PRINT_DEBUG Configuration
-----------------------------------------------------------
 Available log levels (set PRINT_DEBUG_LEVEL via -DPRINT_DEBUG_LEVEL in cmake attributes):
    PRINT_DEBUG_LEVEL_NONE   → disable all logs
    PRINT_DEBUG_LEVEL_ERROR  → show only errors
    PRINT_DEBUG_LEVEL_WARN   → show warnings and errors
    PRINT_DEBUG_LEVEL_INFO   → show info, warnings, and errors

 Available log macros (use in code):
    PRINT_DEBUG_ERROR(...)  → log an error message
    PRINT_DEBUG_WARN(...)   → log a warning message
    PRINT_DEBUG_INFO(...)   → log an info message
===========================================================
*/

#define PRINT_DEBUG_LEVEL_NONE  0
#define PRINT_DEBUG_LEVEL_ERROR 1
#define PRINT_DEBUG_LEVEL_WARN  2
#define PRINT_DEBUG_LEVEL_INFO  3

#ifndef PRINT_DEBUG_LEVEL
#define PRINT_DEBUG_LEVEL PRINT_DEBUG_LEVEL_NONE
#endif

#if PRINT_DEBUG_LEVEL >= PRINT_DEBUG_LEVEL_INFO
#define PRINT_DEBUG_INFO(format, ...) printf("[DEBUG_INFO] " format, ##__VA_ARGS__)
#else
#define PRINT_DEBUG_INFO(format, ...)
#endif

#if PRINT_DEBUG_LEVEL >= PRINT_DEBUG_LEVEL_WARN
#define PRINT_DEBUG_WARN(format, ...) printf("[DEBUG_WARN] " format, ##__VA_ARGS__)
#else
#define PRINT_DEBUG_WARN(format, ...)
#endif

#if PRINT_DEBUG_LEVEL >= PRINT_DEBUG_LEVEL_ERROR
#define PRINT_DEBUG_ERROR(format, ...) do {printf("[DEBUG_ERROR] " format, ##__VA_ARGS__); fprintf(stderr, "[DEBUG_ERROR] " format, ##__VA_ARGS__);} while(0)
#else
#define PRINT_DEBUG_ERROR(format, ...)
#endif


#endif //PRINT_DEBUG_H
