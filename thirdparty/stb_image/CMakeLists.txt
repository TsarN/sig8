add_library(stb_image STATIC
        stb_image.c
        include/stb_image.h)

add_library(stb_image::stb_image ALIAS stb_image)

if (${CMAKE_C_COMPILER_ID} MATCHES "TinyCC")
    target_compile_definitions(stb_image PUBLIC STBI_NO_SIMD)
endif()

set_target_properties(stb_image PROPERTIES C_STANDARD 99)
target_include_directories(stb_image
PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
