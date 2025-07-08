cmake_minimum_required(VERSION 3.24)

# Registers a new service.
# [module_name] is the name of the target
# [folder_name] Name of the folder of the service
function(register_ruken_service service_name folder_name)

    # Creating a symbolic link for includes
    if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR}/Services/${folder_name})
        file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/Sources ${RUKEN_GENERATED_SOURCES_DIR}/Services/${folder_name} SYMBOLIC)
    endif ()

    # Creating the library
    # TODO: Support for hot reload here
    add_library               (${service_name} STATIC)
    target_include_directories(${service_name} PRIVATE ${RUKEN_GENERATED_SOURCES_DIR}/Include)

    # Adding the library to the list of available services
    set_property(GLOBAL APPEND PROPERTY RUKEN_SERVICE_LIBS ${service_name})

endfunction()