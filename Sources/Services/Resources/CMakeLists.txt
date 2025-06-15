cmake_minimum_required(VERSION 3.16.3)

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR}/Services/Resources)
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/Sources ${RUKEN_GENERATED_SOURCES_DIR}/Services/Resources SYMBOLIC)
endif ()

add_library(RkResourcesService STATIC
    Sources/ResourceManager.hpp
        Sources/ResourceManager.cpp
        Sources/Resource.hpp
)

target_include_directories(RkResourcesService PRIVATE ${RUKEN_GENERATED_SOURCES_DIR}/Include)
target_link_libraries     (RkResourcesService PUBLIC RkCore)

set_property(GLOBAL APPEND PROPERTY RUKEN_SERVICE_LIBS RkResourcesService)