add_library(sig8 STATIC sig8.c include/sig8.h)
add_library(sig8::sig8 ALIAS sig8)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

if (MSVC)
    target_compile_options(sig8 PRIVATE /W4)
else()
    target_compile_options(sig8 PRIVATE -Wall -Wextra)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    target_link_options(sig8 PUBLIC -sFULL_ES3=1)
    target_compile_definitions(sig8 PRIVATE SIG8_USE_BUILTIN_ES3)
else()
    find_package(OpenGL REQUIRED)

    target_include_directories(sig8 PRIVATE OpenGL::GL)
    target_link_libraries(sig8 PRIVATE OpenGL::GL glad::glad_gles3)
    target_compile_definitions(sig8 PRIVATE SIG8_USE_GLAD_ES3)
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

target_link_libraries(sig8 PRIVATE m ${CMAKE_DL_LIBS} stb_image::stb_image)
