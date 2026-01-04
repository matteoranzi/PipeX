include_guard(DIRECTORY)

add_library(print_debug INTERFACE)

target_include_directories(print_debug INTERFACE ${CMAKE_SOURCE_DIR}/include)

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



#------------- CUSTOM PRINT DEBUG LEVELS (for console print) -----------------

function(define_print_debug_level_for_target target release_build_level debug_build_level)
    target_compile_definitions(${target} PRIVATE
        $<$<CONFIG:Release>:PRINT_DEBUG_LEVEL=${release_build_level}>
        $<$<CONFIG:Debug>:PRINT_DEBUG_LEVEL=${debug_build_level}>
    )
    show_print_debug_level_for_target(${target} ${release_build_level} ${debug_build_level})
endfunction()

function(show_print_debug_level_for_target target release_level debug_level)
    foreach(conf IN ITEMS Release Debug)
        if(conf STREQUAL Release)
            set(level "${release_level}")
            set(color ${Green})
        else()
            set(level "${debug_level}")
            set(color ${BoldMagenta})
        endif()

        set(_level_name "")
        if(${level} EQUAL PRINT_DEBUG_LEVEL_NONE)
            set(_level_name "${_level_name}|NONE")
        endif()
        if(${level} GREATER_EQUAL PRINT_DEBUG_LEVEL_ERROR)
            set(_level_name "${_level_name}|ERROR")
        endif()
        if(${level} GREATER_EQUAL PRINT_DEBUG_LEVEL_WARN)
            set(_level_name "${_level_name}|WARN")
        endif()
        if(${level} EQUAL PRINT_DEBUG_LEVEL_INFO)
            set(_level_name "${_level_name}|INFO")
        endif()
        set(_level_name "${_level_name}|")

        colored_message("${color}" "[print_debug] Module [${target}] >>> ${conf} PRINT_DEBUG_LEVEL: (${_level_name})")
    endforeach()
endfunction()


#================================================================================================
# Global debug level (used as default for all modules)
set(RELEASE_PRINT_DEBUG_LEVEL ${PRINT_DEBUG_LEVEL_ERROR} CACHE STRING "Global debug level for all modules in release builds")
set(DEBUG_PRINT_DEBUG_LEVEL ${PRINT_DEBUG_LEVEL_INFO} CACHE STRING "Global debug level for all modules in debug builds")
#================================================================================================