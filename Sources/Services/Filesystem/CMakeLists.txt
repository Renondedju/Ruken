cmake_minimum_required(VERSION 3.24)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkService.cmake)
register_ruken_service(RkFilesystemService Filesystem)

# Files
target_sources(RkFilesystemService INTERFACE
        FILE_SET HEADERS BASE_DIRS Sources FILES
            Sources/Filesystem.hpp
            Sources/EFilesystemLocation.hpp
            Sources/DirectoryPath.hpp
            Sources/FilePath.hpp
            Sources/IOJobQueue.hpp
            Sources/File.hpp
            Sources/Windows/WindowsFilesystem.hpp
            Sources/Windows/WindowsFile.hpp
            Sources/FileCursor.hpp
)

target_sources(RkFilesystemService PRIVATE
        Sources/Windows/WindowsFilesystem.cpp
        Sources/Windows/WindowsFile.cpp
        Sources/Filesystem.cpp
)

target_link_libraries(RkFilesystemService PUBLIC RkCore)