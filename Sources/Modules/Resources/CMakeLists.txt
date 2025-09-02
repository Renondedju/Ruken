cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkModule.cmake)
register_ruken_module(RkResources)

# Files
target_sources(RkResources PUBLIC
    FILE_SET HEADERS FILES
        Resource.hpp
        ResourceHandle.hpp
        ResourceLoader.hpp
        ResourceManifest.hpp
        ResourceHandle.inl
        Assets/AssetImporter.hpp
        Assets/ImportContext.hpp
        ResourceManager.hpp
)

target_sources(RkResources PRIVATE
        ResourceManager.cpp
        Assets/AssetImporter.cpp
)

target_link_libraries(RkResources PUBLIC RkCore)