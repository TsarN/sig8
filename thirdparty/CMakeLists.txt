add_subdirectory(stb_ds)
add_subdirectory(stb_image)
add_subdirectory(stb_image_write)

if (NOT (${CMAKE_SYSTEM_NAME} MATCHES "Emscripten"))
    add_subdirectory(glad)
endif()

add_subdirectory(glfw)
