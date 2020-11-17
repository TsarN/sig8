set(EXAMPLE_NAME hello)
add_executable(${EXAMPLE_NAME} main.c)
target_link_libraries(${EXAMPLE_NAME} sig8::sig8)
sig8_bundle(${EXAMPLE_NAME} RESOURCES main.c)
