# --- Sanitizers

if (RUKEN_ASAN_ENABLED OR RUKEN_TSAN_ENABLED OR RUKEN_UBSAN_ENABLED)
    string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")
endif()

if(RUKEN_ASAN_ENABLED)
    add_compile_options(-fsanitize=address)
    add_link_options   (-fsanitize=address)
endif()

if(RUKEN_TSAN_ENABLED)
    add_compile_options(-fsanitize=thread)
    add_link_options   (-fsanitize=thread)
endif()

if(RUKEN_UBSAN_ENABLED)
    add_compile_options(-fsanitize=undefined)
    add_link_options   (-fsanitize=undefined)
endif()

# --- Modules

add_subdirectory(${RUKEN_MODULES_DIR}/Core)
add_subdirectory(${RUKEN_MODULES_DIR}/ECS)
add_subdirectory(${RUKEN_MODULES_DIR}/Resources)
add_subdirectory(${RUKEN_MODULES_DIR}/Filesystem)

if (NOT ${RUKEN_HEADLESS_BUILD})
    add_subdirectory(${RUKEN_MODULES_DIR}/Rendering)
endif ()