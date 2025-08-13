cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkModule.cmake)
register_ruken_module(RkFilesystem)

# Files
target_sources(RkFilesystem PUBLIC
        FILE_SET HEADERS FILES
    Filesystem.hpp
    EFilesystemLocation.hpp
        FilesystemPath.hpp
    IOJobQueue.hpp
    File.hpp
    Windows/WindowsFilesystem.hpp
    Windows/WindowsFile.hpp
    FileCursor.hpp
)

target_sources(RkFilesystem PRIVATE
    Windows/WindowsFilesystem.cpp
    Windows/WindowsFile.cpp
    Filesystem.cpp
)

target_link_libraries(RkFilesystem PUBLIC RkCore)