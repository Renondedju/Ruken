add_subdirectory(${RUKEN_SERVICES_DIR}/ECS)

if (NOT ${RUKEN_HEADLESS_BUILD})
    add_subdirectory(${RUKEN_SERVICES_DIR}/Rendering)
endif ()