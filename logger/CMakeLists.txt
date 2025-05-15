file(GLOB_RECURSE Log log.cpp log.hpp)

add_library(Logger STATIC ${Log})

target_include_directories(Logger PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/includes)

target_link_directories(Logger PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/libs)

target_link_libraries(Logger libspdlog.a)