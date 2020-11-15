add_library(sig8 STATIC sig8.c include/sig8.h)
add_library(sig8::sig8 ALIAS sig8)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h")

if (MSVC)
    target_compile_options(sig8 PRIVATE /W4)
else()
    target_compile_options(sig8 PRIVATE -Wall -Wextra)
endif()

set_target_properties(sig8 PROPERTIES
    VERSION ${SIG8_VERSION}
    C_STANDARD 99
)

target_include_directories(sig8
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"
)
