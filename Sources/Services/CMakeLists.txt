add_subdirectory(${RUKEN_SERVICES_DIR}/ECS)
add_subdirectory(${RUKEN_SERVICES_DIR}/Resources)
add_subdirectory(${RUKEN_SERVICES_DIR}/Filesystem)

if (NOT ${RUKEN_HEADLESS_BUILD})
    # add_subdirectory(${RUKEN_SERVICES_DIR}/Rendering)
endif ()