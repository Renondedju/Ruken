cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkModule.cmake)
register_ruken_module(RkECS)

# Files
target_sources(RkECS PUBLIC
        FILE_SET HEADERS FILES
    Archetype.hpp
    Archetype.inl
        ComponentFingerprint.hpp
        ComponentFingerprint.inl
    Components/EntityComponent.hpp
    Components/EntityComponent.inl
    Components/Component.hpp
    ComponentQuery.hpp
    ComponentQuery.inl
    ComponentView.hpp
    ComponentView.inl
    EEventName.hpp
    Entity.hpp
    Universe.hpp
    Universe.inl
    SystemEventHandler.hpp
    Components/UniverseComponent.hpp
    Range.hpp
    System.hpp
    System.inl
    Components/TagComponent.hpp
    Meta/ItemHelper.hpp
    Meta/ComponentHelper.hpp
)

target_sources(RkECS PRIVATE
    Archetype.cpp
    Entity.cpp
    SystemEventHandler.cpp
    Universe.cpp
    Range.cpp
    Components/TagComponent.cpp
    System.cpp
    ComponentQuery.cpp
    Components/Component.cpp
)

if (MSVC)
    target_compile_options(RkECS PUBLIC /Zc:preprocessor)
endif ()

target_link_libraries     (RkECS PUBLIC RkCore)
