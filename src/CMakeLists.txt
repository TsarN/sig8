add_library(sig8 STATIC sig8.c include/sig8.h)
add_library(sig8::sig8 ALIAS sig8)

if (MSVC)
    target_compile_options(sig8 PRIVATE /W4)
else()
    target_compile_options(sig8 PRIVATE -Wall -Wextra)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    target_link_options(sig8 PUBLIC -sFULL_ES3=1)
    target_compile_definitions(sig8 PRIVATE)
else()
    target_include_directories(sig8 PRIVATE)
    target_link_libraries(sig8 PRIVATE glad::glad)
    target_compile_definitions(sig8 PRIVATE SIG8_USE_GLAD)
endif()

target_link_libraries(sig8 PRIVATE glfw::glfw)

if (${CMAKE_C_COMPILER_ID} MATCHES "TinyCC")
    target_compile_definitions(sig8 PRIVATE SDL_DISABLE_IMMINTRIN_H)
endif()

set_target_properties(sig8 PROPERTIES
    VERSION ${SIG8_VERSION}
    C_STANDARD 99
)

target_include_directories(sig8
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(sig8 PRIVATE
        m
        ${CMAKE_DL_LIBS}
        stb_ds::stb_ds
        stb_image::stb_image
)

sig8_bundle(sig8
    FORCE
    NAME sig8_SystemBundle
    FILE sig8_system_resources
    RESOURCES
        ./resources/default_palette.png
        ./resources/default_font.png
)
