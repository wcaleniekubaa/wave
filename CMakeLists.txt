cmake_minimum_required(VERSION 3.27)

set(PROJECT_VERSION "0.1.0")
set(PROJECT_VERSION_MAJOR "0")

project(wave VERSION ${PROJECT_VERSION})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WAVE_BUILD_EXAMPLES "Build examples in ./examples" ON)

set(WAVE_SOURCES
        include/wave/wave.h
        include/wave/wave.hpp
        src/wave.c)

if (UNIX)
    link_libraries(m)
endif ()

if (MINGW)
    link_libraries(-static-libgcc)
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(wave_static STATIC ${WAVE_SOURCES})
add_library(wave_shared SHARED ${WAVE_SOURCES})

if (WAVE_BUILD_EXAMPLES)
    add_executable(read_wav examples/read_wav/main.c)
    target_link_libraries(read_wav wave_shared)

    add_executable(write_empty_wav examples/write_empty_wav/main.c)
    target_link_libraries(write_empty_wav wave_shared)

    add_executable(rewrite_wav examples/rewrite_wav/main.c)
    target_link_libraries(rewrite_wav wave_shared)

    add_executable(cpp_wrapper examples/cpp_wrapper/main.cpp
            examples/cpp_wrapper/main.cpp)
    target_link_libraries(cpp_wrapper wave_shared)
endif ()

target_compile_definitions(wave_static PRIVATE WAVE_STATIC)
target_compile_definitions(wave_shared PRIVATE WAVE_SHARED WAVE_EXPORT)

if (MINGW)
    set_target_properties(wave_static PROPERTIES PREFIX "")
    set_target_properties(wave_shared PROPERTIES PREFIX "")
    set_target_properties(wave_static PROPERTIES OUTPUT_NAME wave)
    set_target_properties(wave_shared PROPERTIES OUTPUT_NAME wave)
endif ()