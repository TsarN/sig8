add_library(glad_gles3 STATIC
        glad.c
        include/glad.h)

add_library(glad::glad_gles3 ALIAS glad_gles3)

target_compile_features(glad_gles3 PRIVATE c_std_99)
target_include_directories(glad_gles3
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
