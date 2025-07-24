add_subdirectory(${RUKEN_MODULES_DIR}/Core)
add_subdirectory(${RUKEN_MODULES_DIR}/ECS)
add_subdirectory(${RUKEN_MODULES_DIR}/Resources)
add_subdirectory(${RUKEN_MODULES_DIR}/Filesystem)

if (NOT ${RUKEN_HEADLESS_BUILD})
    add_subdirectory(${RUKEN_MODULES_DIR}/Rendering)
endif ()