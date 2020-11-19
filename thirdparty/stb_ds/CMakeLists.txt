add_library(stb_ds STATIC
        stb_ds.c
        include/stb_ds.h)

add_library(stb_ds::stb_ds ALIAS stb_ds)

set_target_properties(stb_ds PROPERTIES C_STANDARD 99)
target_include_directories(stb_ds
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
