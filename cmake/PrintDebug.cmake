include_guard(DIRECTORY)

add_library(print_debug INTERFACE)

target_include_directories(print_debug INTERFACE ${CMAKE_SOURCE_DIR}/include)


#------------- CUSTOM PRINT DEBUG LEVELS (for console print) -----------------
#[[
 Available log levels - set PRINT_DEBUG_LEVEL to one of:
    PRINT_DEBUG_LEVEL_NONE   → disable all logs
    PRINT_DEBUG_LEVEL_ERROR  → show only errors
    PRINT_DEBUG_LEVEL_WARN   → show warnings and errors
    PRINT_DEBUG_LEVEL_INFO   → show info, warnings, and errors
]]
set(PRINT_DEBUG_LEVEL_NONE  0)
set(PRINT_DEBUG_LEVEL_ERROR 1)
set(PRINT_DEBUG_LEVEL_WARN  2)
set(PRINT_DEBUG_LEVEL_INFO  3)

function(define_print_debug_level_for_target target level)
    target_compile_definitions(${target} PRIVATE PRINT_DEBUG_LEVEL=${level})
    show_print_debug_level_for_target(${target} ${level})
endfunction()

function(show_print_debug_level_for_target target level)
    set(_level_name "")
    if(level EQUAL PRINT_DEBUG_LEVEL_NONE)
        set(_level_name "${_level_name}|NONE")
    endif ()
    if(level GREATER_EQUAL PRINT_DEBUG_LEVEL_ERROR)
        set(_level_name "${_level_name}|ERROR")
    endif ()
    if(level GREATER_EQUAL PRINT_DEBUG_LEVEL_WARN)
        set(_level_name "${_level_name}|WARN")
    endif ()
    if(level EQUAL PRINT_DEBUG_LEVEL_INFO)
        set(_level_name "${_level_name}|INFO")
    endif ()
    set(_level_name "${_level_name}|")

    colored_message(${BoldMagenta} "[print_debug] Module [${target}] >>> PRINT_DEBUG_LEVEL = ${level} (${_level_name})")
endfunction()