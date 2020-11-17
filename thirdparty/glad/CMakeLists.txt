add_library(glad STATIC
        glad.c
        include/glad.h)

add_library(glad::glad ALIAS glad)

set_target_properties(glad PROPERTIES C_STANDARD 99)
target_include_directories(glad
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
