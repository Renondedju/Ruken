cmake_minimum_required(VERSION 3.24)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkService.cmake)
register_ruken_service(RkECSService ECS)

# Files
target_sources(RkECSService INTERFACE
        FILE_SET HEADERS BASE_DIRS Sources FILES
            Sources/Test/CounterSystem.hpp
            Sources/Archetype.hpp
            Sources/Archetype.inl
            Sources/ArchetypeFingerprint.hpp
            Sources/ArchetypeFingerprint.inl
            Sources/Component.hpp
            Sources/Component.inl
            Sources/ComponentBase.hpp
            Sources/ComponentCounter.hpp
            Sources/ComponentField.hpp
            Sources/ComponentQuery.hpp
            Sources/ComponentQuery.inl
            Sources/ComponentView.hpp
            Sources/ComponentView.inl
            Sources/EEventName.hpp
            Sources/Entity.hpp
            Sources/EntityAdmin.hpp
            Sources/EntityAdmin.inl
            Sources/EventHandler.hpp
            Sources/EventHandler.inl
            Sources/EventHandlerBase.hpp
            Sources/ExclusiveComponent.hpp
            Sources/Range.hpp
            Sources/System.hpp
            Sources/System.inl
            Sources/TagComponent.hpp
            Sources/Test/CounterComponent.hpp
            Sources/Safety/ViewType.hpp
            Sources/Safety/TagComponentType.hpp
            Sources/Safety/SystemType.hpp
            Sources/Safety/ExclusiveComponentType.hpp
            Sources/Safety/EventHandlerType.hpp
            Sources/Safety/ComponentType.hpp
            Sources/Safety/ComponentFieldType.hpp
            Sources/Safety/AnyComponentType.hpp
            Sources/Meta/ItemHelper.hpp
            Sources/Meta/FieldHelper.hpp
            Sources/Meta/ComponentHelper.hpp
)

target_sources(RkECSService PRIVATE
        Sources/Archetype.cpp
        Sources/Entity.cpp
        Sources/EventHandlerBase.cpp
        Sources/EntityAdmin.cpp
        Sources/Range.cpp
        Sources/TagComponent.cpp
        Sources/System.cpp
        Sources/ComponentQuery.cpp
        Sources/ComponentBase.cpp
)

if (MSVC)
    target_compile_options(RkECSService PUBLIC /Zc:preprocessor)
endif ()

target_link_libraries     (RkECSService PUBLIC RkCore)
