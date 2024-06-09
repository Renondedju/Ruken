cmake_minimum_required(VERSION 3.16.3)

if (NOT EXISTS ${RUKEN_GENERATED_SOURCES_DIR}/Services/ECS)
    file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/Sources ${RUKEN_GENERATED_SOURCES_DIR}/Services/ECS SYMBOLIC)
endif ()

add_library(RkECSService STATIC
        Sources/Test/CounterSystem.hpp
        Sources/Archetype.cpp
        Sources/Archetype.hpp
        Sources/Archetype.inl
        Sources/ArchetypeFingerprint.hpp
        Sources/ArchetypeFingerprint.inl
        Sources/Component.hpp
        Sources/Component.inl
        Sources/ComponentBase.cpp
        Sources/ComponentBase.hpp
        Sources/ComponentCounter.hpp
        Sources/ComponentField.hpp
        Sources/ComponentQuery.cpp
        Sources/ComponentQuery.hpp
        Sources/ComponentQuery.inl
        Sources/ComponentView.hpp
        Sources/ComponentView.inl
        Sources/EEventName.hpp
        Sources/Entity.cpp
        Sources/Entity.hpp
        Sources/EntityAdmin.cpp
        Sources/EntityAdmin.hpp
        Sources/EntityAdmin.inl
        Sources/EventHandler.hpp
        Sources/EventHandler.inl
        Sources/EventHandlerBase.cpp
        Sources/EventHandlerBase.hpp
        Sources/ExclusiveComponent.hpp
        Sources/Range.cpp
        Sources/Range.hpp
        Sources/System.cpp
        Sources/System.hpp
        Sources/System.inl
        Sources/TagComponent.cpp
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

target_include_directories(RkECSService PRIVATE ${RUKEN_GENERATED_SOURCES_DIR}/Include)
target_include_directories(RkECSService PRIVATE ${RUKEN_SOURCES_DIR}/Core/Include)
target_include_directories(RkECSService PRIVATE ${RUKEN_GENERATED_SOURCES_DIR}/Services)

target_link_libraries(RkECSService PUBLIC RkCore)