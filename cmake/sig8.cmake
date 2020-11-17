function(sig8_bundle target)
    cmake_parse_arguments(
            SIG8_BUNDLE
            "FORCE"
            "NAME;FILE"
            "RESOURCES"
            ${ARGN}
    )

    set(SIG8_DEFAULT_BUNDLE NO)

    if (NOT DEFINED SIG8_BUNDLE_NAME)
        set(SIG8_BUNDLE_NAME SIG8_RESOURCE_BUNDLE)
        set(SIG8_DEFAULT_BUNDLE YES)
    endif()

    if (NOT DEFINED SIG8_BUNDLE_FILE)
        set(SIG8_BUNDLE_FILE sig8_resources)
    endif()

    if (NOT DEFINED SIG8_BUNDLE_RESOURCES)
        set(SIG8_BUNDLE_RESOURCES "")
    endif()

    if ((${CMAKE_SYSTEM_NAME} MATCHES "Emscripten") AND (NOT SIG8_BUNDLE_FORCE))
        foreach(RES ${SIG8_BUNDLE_RESOURCES})
            target_link_options(${target} PRIVATE
                    "SHELL:--preload-file \"${CMAKE_CURRENT_SOURCE_DIR}/${RES}@${RES}\""
                    )
        endforeach()
        target_compile_definitions(${target} PRIVATE SIG8_USE_RESOURCE_PATH="")
    elseif ((${CMAKE_BUILD_TYPE} MATCHES "Release") OR (SIG8_BUNDLE_FORCE))
        set(SIG8_BUNDLE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${SIG8_BUNDLE_FILE}")
        set(SIG8_RESOURCE_PATHS "")
        set(SIG8_RESOURCE_NAMES "")

        foreach(RES ${SIG8_BUNDLE_RESOURCES})
            get_filename_component(RES_PATH "${RES}" ABSOLUTE BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
            file(RELATIVE_PATH RES_NAME "${CMAKE_CURRENT_SOURCE_DIR}" "${RES_PATH}")
            list(APPEND SIG8_RESOURCE_PATHS "${RES_PATH}")
            list(APPEND SIG8_RESOURCE_NAMES "${RES_NAME}")
        endforeach()

        string (REPLACE ";" "$<SEMICOLON>" SIG8_RESOURCE_PATHS_ESCAPED "${SIG8_RESOURCE_PATHS}")
        string (REPLACE ";" "$<SEMICOLON>" SIG8_RESOURCE_NAMES_ESCAPED "${SIG8_RESOURCE_NAMES}")

        add_custom_command(
                OUTPUT "${SIG8_BUNDLE_PATH}.c"
                COMMAND "${CMAKE_COMMAND}"
                -DSIG8_BUNDLE_NAME=${SIG8_BUNDLE_NAME}
                -DSIG8_BUNDLE_PATH=${SIG8_BUNDLE_PATH}.c
                -DSIG8_RESOURCE_PATHS=${SIG8_RESOURCE_PATHS_ESCAPED}
                -DSIG8_RESOURCE_NAMES=${SIG8_RESOURCE_NAMES_ESCAPED}
                -P "${SIG8_ROOT}/cmake/makebundle.cmake"
                DEPENDS ${SIG8_ROOT}/cmake/makebundle.cmake ${SIG8_BUNDLE_RESOURCES}
                COMMENT "Generating bundles for target ${target}"
                VERBATIM
        )

        if (SIG8_DEFAULT_BUNDLE)
            target_compile_definitions(${target} PRIVATE SIG8_USE_DEFAULT_BUNDLE)
        else()
            string(MAKE_C_IDENTIFIER "${SIG8_BUNDLE_PATH}" SIG8_BUNDLE_PATH_IDENT)
            file(WRITE "${SIG8_BUNDLE_PATH}.h"
"#ifndef ${SIG8_BUNDLE_PATH_IDENT}_H_INCLUDED
#define ${SIG8_BUNDLE_PATH_IDENT}_H_INCLUDED

extern const unsigned char *${SIG8_BUNDLE_NAME};

#endif
")

            target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
            target_sources(${target} PRIVATE "${SIG8_BUNDLE_PATH}.h")
        endif()

        target_sources(${target} PRIVATE "${SIG8_BUNDLE_PATH}.c")
    else()
        target_compile_definitions(${target} PRIVATE SIG8_USE_RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")
    endif()
endfunction()

function(sig8_main target)
    target_sources(${target} PRIVATE "${SIG8_ROOT}/src/sig8_main.c")
endfunction()

function(sig8_link target)
    target_link_libraries(${target} PRIVATE sig8::sig8)
endfunction()
