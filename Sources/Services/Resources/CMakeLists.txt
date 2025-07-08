cmake_minimum_required(VERSION 3.24)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkService.cmake)
register_ruken_service(RkResourcesService Resources)

# Files
target_sources(RkResourcesService INTERFACE
    FILE_SET HEADERS BASE_DIRS Sources FILES
        Sources/Resource.hpp
        Sources/ResourceManager.hpp
)

target_sources(RkResourcesService PRIVATE
        Sources/ResourceManager.cpp
)

target_link_libraries(RkResourcesService PUBLIC RkCore)