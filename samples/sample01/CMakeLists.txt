add_executable(Sample01 main.cpp)

target_include_directories(Sample01 PUBLIC ${CMAKE_SOURCE_DIR}/source/include)

target_compile_definitions(Sample01 PUBLIC
    -DGLM_FORCE_DEPTH_ZERO_TO_ONE
    -DGLM_FORCE_RADIANS
    -DGLM_FORCE_LEFT_HANDED
)

target_link_libraries(Sample01 CPUPathTracing  glm stdc++fs Logger)