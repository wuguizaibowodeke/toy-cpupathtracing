add_executable(SandBox main.cpp)

target_include_directories(SandBox PUBLIC ${CMAKE_SOURCE_DIR}/source/include)
target_include_directories(SandBox PUBLIC ${CMAKE_SOURCE_DIR}/logger)


target_compile_definitions(SandBox PUBLIC
    -DGLM_FORCE_DEPTH_ZERO_TO_ONE
    -DGLM_FORCE_RADIANS
    -DGLM_FORCE_LEFT_HANDED
    SPDLOG_COMPILED_LIB
)

target_link_libraries(SandBox CPUPathTracing  glm stdc++fs Logger)