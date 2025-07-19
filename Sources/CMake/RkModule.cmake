cmake_minimum_required(VERSION 3.28)

# Registers a new service.
# [module_name] is the name of the target
function(register_ruken_module module_name)

    # Creating the library
    # TODO: Support for hot reload here
    add_library               (${module_name} STATIC)
    target_include_directories(${module_name} PUBLIC  ${RUKEN_MODULES_DIR})
    target_include_directories(${module_name} PRIVATE ${RUKEN_GENERATED_SOURCES_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

    # Adding the library to the list of available services
    set_property(GLOBAL APPEND PROPERTY RUKEN_MODULES_LIB ${module_name})

endfunction()