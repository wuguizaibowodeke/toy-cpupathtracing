cmake_minimum_required(VERSION 3.10)

project(
    CPUPathTracing
    LANGUAGES CXX
)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

# 控制是否编译 samples
option(BUILD_SAMPLES "Build sample programs" OFF)
#set(BUILD_SAMPLES ON CACHE BOOL "Build sample programs" FORCE)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/rapidobj/include
)

add_subdirectory(source)
add_subdirectory(thirdparty/glm)
add_subdirectory(thirdparty/rapidobj)

add_subdirectory(samples/sandbox)

if (BUILD_SAMPLES)
    add_subdirectory(samples/sample01)
    add_subdirectory(samples/sample02)
endif()