# Cmake version requirements
cmake_minimum_required(VERSION 3.4)

# Project name
project(photo_c)

# Parameters of the compiler
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm")

# Source files, which will be used by the final file -> photoshop
add_executable(photoshop main.c edit_functions.c parameters_control.c)

# Checking for math.h
include(CheckFunctionExists)

if(NOT POW_FUNCTION_EXISTS AND NOT NEED_LINKING_AGAINST_LIBM)
    CHECK_FUNCTION_EXISTS(pow POW_FUNCTION_EXISTS)

    if(NOT POW_FUNCTION_EXISTS)
        unset(POW_FUNCTION_EXISTS CACHE)
        list(APPEND CMAKE_REQUIRED_LIBRARIES m)
        CHECK_FUNCTION_EXISTS(pow POW_FUNCTION_EXISTS)

        if(POW_FUNCTION_EXISTS)
            set(NEED_LINKING_AGAINST_LIBM True CACHE BOOL "" FORCE)
        else()
            message(FATAL_ERROR "Failed making the pow() function available")
        endif()
    endif()
endif()

if(NEED_LINKING_AGAINST_LIBM)
    target_link_libraries(photoshop m)
endif()