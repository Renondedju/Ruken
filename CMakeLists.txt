cmake_minimum_required(VERSION 3.16.3)

# --- Language setup
set(CMAKE_CXX_STANDARD 23)

# --- Project setup
project(Ruken)

macro(set_bool var)
    if(${ARGN})
        set(${var} ON)
    else()
        set(${var} OFF)
    endif()
endmacro()

set     (RUKEN_DEBUG_BUILD_TYPES     Debug RelWithDebInfo)
set_bool(RUKEN_BUILD_HAS_DEBUG_INFO  ${CMAKE_BUILD_TYPE} IN_LIST RUKEN_DEBUG_BUILD_TYPES)
set     (RUKEN_GENERATED_SOURCES_DIR ${CMAKE_BINARY_DIR}/Generated)
set     (RUKEN_SOURCES_DIR           ${CMAKE_CURRENT_LIST_DIR}/Sources)
set     (RUKEN_SERVICES_DIR          ${RUKEN_SOURCES_DIR}/Services)
set     (RUKEN_CORE_DIR              ${RUKEN_SOURCES_DIR}/Core)

option(RUKEN_HEADLESS_BUILD          "Disables all form of rendering and presentation" OFF)
option(RUKEN_TRACE_BUILD             "Enables support for the tracy profiler" ${RUKEN_BUILD_HAS_DEBUG_INFO})
option(RUKEN_BREAK_ON_ERROR          "When on, a break interrupt will be generated on errors" ${RUKEN_BUILD_HAS_DEBUG_INFO})
option(RUKEN_TRACE_SHOW_WORKER_ZONES "When on, tracy will also report the activity of workers" OFF)
option(RUKEN_TRACE_SHOW_LOGGER_ZONES "When on, tracy will also report the activity of loggers" OFF)

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR})
    file(MAKE_DIRECTORY ${RUKEN_GENERATED_SOURCES_DIR})
endif ()

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR}/Services)
    file(MAKE_DIRECTORY ${RUKEN_GENERATED_SOURCES_DIR}/Services)
endif ()

# --- Add dependencies
include(${RUKEN_SOURCES_DIR}/CMake/CPM.cmake)
define_property(GLOBAL PROPERTY RUKEN_SERVICE_LIBS)

set(CPM_SOURCE_CACHE ${RUKEN_SOURCES_DIR}/ThirdParty)

file(LOCK ${CPM_SOURCE_CACHE} DIRECTORY GUARD FILE)
add_subdirectory(${RUKEN_SERVICES_DIR})
add_subdirectory(${RUKEN_CORE_DIR})
file(LOCK ${CPM_SOURCE_CACHE} DIRECTORY GUARD FILE RELEASE)

get_property(RUKEN_SERVICE_LIBS GLOBAL PROPERTY RUKEN_SERVICE_LIBS)

# --- Executable

add_compile_definitions(UNICODE _UNICODE)
add_executable(Ruken Sources/Core/Src/Main.cpp
        Sources/Src/Test.hpp
)

target_compile_definitions(Ruken PRIVATE RUKEN_PROJECT_NAME="Ruken")

target_link_libraries(Ruken RkCore ${RUKEN_SERVICE_LIBS})