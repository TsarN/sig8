add_library(glad_gles3 STATIC
        glad.c
        include/glad.h)

add_library(glad::glad_gles3 ALIAS glad_gles3)

set_target_properties(glad_gles3 PROPERTIES C_STANDARD 99)
target_include_directories(glad_gles3
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
