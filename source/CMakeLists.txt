file(GLOB_RECURSE Source src/*.cpp src/*.h src/*.hpp)
#add_executable(CPUPathTracing ${Source})
add_library(CPUPathTracing STATIC ${Source})
target_include_directories(CPUPathTracing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(CPUPathTracing PUBLIC
    -DGLM_FORCE_DEPTH_ZERO_TO_ONE
    -DGLM_FORCE_RADIANS
    -DGLM_FORCE_LEFT_HANDED
)
target_link_libraries(CPUPathTracing glm stdc++fs rapidobj)