cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkModule.cmake)
register_ruken_module(RkFilesystem)

# Files
target_sources(RkFilesystem PUBLIC
        FILE_SET HEADERS FILES
    Filesystem.hpp
    EFilesystemLocation.hpp
        FilePath.hpp
    IOJobQueue.hpp
    File.hpp
        STD/StdFilesystem.hpp
        STD/StdFile.hpp
    FileCursor.hpp
)

target_sources(RkFilesystem PRIVATE
        STD/StdFilesystem.cpp
        STD/StdFile.cpp
    Filesystem.cpp
)

target_link_libraries(RkFilesystem PUBLIC RkCore)