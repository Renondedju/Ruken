cmake_minimum_required(VERSION 3.28)

# Registers a new service.
# [MODULE_NAME] is the name of the target
function(register_ruken_module MODULE_NAME)

    # TODO: - Support hot reload
    #       - Support config files
    #       - Support per-module tracing

    # Creating the library
    add_library               (${MODULE_NAME} STATIC)
    target_include_directories(${MODULE_NAME} PUBLIC  ${RUKEN_MODULES_DIR})
    target_include_directories(${MODULE_NAME} PRIVATE ${RUKEN_GENERATED_SOURCES_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

    # Enabling warnings
    if(MSVC)
        target_compile_options(${MODULE_NAME} PRIVATE /W4)
    else()
        target_compile_options(${MODULE_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    # Adding the library to the list of available services
    set_property(GLOBAL APPEND PROPERTY RUKEN_MODULES_LIB ${MODULE_NAME})

endfunction()