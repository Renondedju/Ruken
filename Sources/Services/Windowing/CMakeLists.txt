cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkService.cmake)
register_ruken_service(RkWindowingService Windowing)

# Dependancies
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)

CPMAddPackage(URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip" NAME glfw)

# Files
target_sources(RkWindowingService INTERFACE
    FILE_SET HEADERS BASE_DIRS Sources FILES
        Sources/WindowManager.hpp
        Sources/Window.hpp
)

target_sources(RkWindowingService PRIVATE
        Sources/WindowManager.cpp
        Sources/Window.cpp
)

target_link_libraries(RkWindowingService PUBLIC glfw RkCore)