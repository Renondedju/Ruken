cmake_minimum_required(VERSION 3.28)

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
set     (RUKEN_MODULES_DIR           ${RUKEN_SOURCES_DIR}/Modules)
set     (RUKEN_CORE_DIR              ${RUKEN_MODULES_DIR}/Core)

option(RUKEN_HEADLESS_BUILD          "Disables all form of rendering and presentation" OFF)
option(RUKEN_TRACE_BUILD             "Enables support for the tracy profiler" ${RUKEN_BUILD_HAS_DEBUG_INFO})
option(RUKEN_BREAK_ON_ERROR          "When on, a break interrupt will be generated on errors" ${RUKEN_BUILD_HAS_DEBUG_INFO})
option(RUKEN_TRACE_SHOW_WORKER_ZONES "When on, tracy will also report the activity of workers" OFF)
option(RUKEN_TRACE_SHOW_LOGGER_ZONES "When on, tracy will also report the activity of loggers" OFF)

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR})
    file(MAKE_DIRECTORY ${RUKEN_GENERATED_SOURCES_DIR})
endif ()

# --- Add dependencies
include(${RUKEN_SOURCES_DIR}/CMake/CPM.cmake)
define_property(GLOBAL PROPERTY RUKEN_MODULES_LIBS)

set(CPM_SOURCE_CACHE ${RUKEN_SOURCES_DIR}/ThirdParty)

file(LOCK ${CPM_SOURCE_CACHE} DIRECTORY GUARD FILE)
    add_subdirectory(${RUKEN_MODULES_DIR})
file(LOCK ${CPM_SOURCE_CACHE} DIRECTORY GUARD FILE RELEASE)

get_property(RUKEN_MODULES_LIB GLOBAL PROPERTY RUKEN_MODULES_LIB)

# --- Executable

add_compile_definitions(UNICODE _UNICODE)
add_executable(Ruken
    Sources/App/Main.cpp
    Sources/App/Test.hpp
)

target_compile_definitions(Ruken PRIVATE RUKEN_PROJECT_NAME="Ruken")
target_include_directories(Ruken PRIVATE ${RUKEN_MODULES_DIR} ${RUKEN_GENERATED_SOURCES_DIR})

target_link_libraries(Ruken ${RUKEN_MODULES_LIB})