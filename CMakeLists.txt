cmake_minimum_required(VERSION 3.10)

project(
    CPUPathTracing
    LANGUAGES CXX
)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glm
)

add_subdirectory(source)
add_subdirectory(thirdparty/glm)