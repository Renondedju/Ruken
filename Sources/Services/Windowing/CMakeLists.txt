cmake_minimum_required(VERSION 3.28)

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR}/Services/Windowing)
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/Sources ${RUKEN_GENERATED_SOURCES_DIR}/Services/Windowing SYMBOLIC)
endif ()

set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)

CPMAddPackage(URL "https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip" NAME glfw)

add_library(RkWindowingService STATIC
        Sources/WindowManager.hpp
        Sources/WindowManager.cpp
        Sources/Window.hpp
        Sources/Window.cpp
)

target_include_directories(RkWindowingService PRIVATE ${RUKEN_GENERATED_SOURCES_DIR}/Include)
target_link_libraries     (RkWindowingService PUBLIC  glfw RkCore)

set_property(GLOBAL APPEND PROPERTY RUKEN_SERVICE_LIBS RkWindowingService)