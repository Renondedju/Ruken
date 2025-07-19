cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkModule.cmake)
register_ruken_module(RkECS)

# Files
target_sources(RkECS INTERFACE
        FILE_SET HEADERS FILES
    Test/CounterSystem.hpp
    Archetype.hpp
    Archetype.inl
    ArchetypeFingerprint.hpp
    ArchetypeFingerprint.inl
    Component.hpp
    Component.inl
    ComponentBase.hpp
    ComponentCounter.hpp
    ComponentField.hpp
    ComponentQuery.hpp
    ComponentQuery.inl
    ComponentView.hpp
    ComponentView.inl
    EEventName.hpp
    Entity.hpp
    EntityAdmin.hpp
    EntityAdmin.inl
    EventHandler.hpp
    EventHandler.inl
    EventHandlerBase.hpp
    ExclusiveComponent.hpp
    Range.hpp
    System.hpp
    System.inl
    TagComponent.hpp
    Test/CounterComponent.hpp
    Safety/ViewType.hpp
    Safety/TagComponentType.hpp
    Safety/SystemType.hpp
    Safety/ExclusiveComponentType.hpp
    Safety/EventHandlerType.hpp
    Safety/ComponentType.hpp
    Safety/ComponentFieldType.hpp
    Safety/AnyComponentType.hpp
    Meta/ItemHelper.hpp
    Meta/FieldHelper.hpp
    Meta/ComponentHelper.hpp
)

target_sources(RkECS PRIVATE
    Archetype.cpp
    Entity.cpp
    EventHandlerBase.cpp
    EntityAdmin.cpp
    Range.cpp
    TagComponent.cpp
    System.cpp
    ComponentQuery.cpp
    ComponentBase.cpp
)

if (MSVC)
    target_compile_options(RkECS PUBLIC /Zc:preprocessor)
endif ()

target_link_libraries     (RkECS PUBLIC RkCore)
