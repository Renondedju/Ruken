cmake_minimum_required(VERSION 3.16.3)

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR}/Services/Filesystem)
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/Sources ${RUKEN_GENERATED_SOURCES_DIR}/Services/Filesystem SYMBOLIC)
endif ()

add_library(RkFilesystemService STATIC
    Sources/Filesystem.hpp
    Sources/EFilesystemLocation.hpp
        Sources/DirectoryPath.hpp
        Sources/FilePath.hpp
        Sources/EOpenMode.hpp
        Sources/IOJobQueue.hpp
        Sources/File.hpp
        Sources/Windows/WindowsFilesystem.hpp
        Sources/Windows/WindowsFilesystem.cpp
        Sources/Windows/WindowsFile.cpp
        Sources/Windows/WindowsFile.hpp
)

target_include_directories(RkFilesystemService PRIVATE ${RUKEN_GENERATED_SOURCES_DIR}/Include)
target_link_libraries     (RkFilesystemService PUBLIC RkCore)

set_property(GLOBAL APPEND PROPERTY RUKEN_SERVICE_LIBS RkFilesystemService)