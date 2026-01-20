cmake_minimum_required(VERSION 3.28)

# Creating a new service
include(${RUKEN_SOURCES_DIR}/CMake/RkModule.cmake)
register_ruken_module(RkECS)

# Files
target_sources(RkECS PUBLIC
        FILE_SET HEADERS FILES
    Archetype.hpp
    Archetype.inl
    Components/ComponentFingerprint.hpp
    Components/ComponentFingerprint.inl
    Components/EntityComponent.hpp
    Components/EntityComponent.inl
    Components/Component.hpp
    Components/ComponentQuery.hpp
    EEventName.hpp
    Entity.hpp
    Universe.hpp
    Universe.inl
    Systems/SystemEventHandler.hpp
    Components/UniverseComponent.hpp
    Range.hpp
    Systems/System.hpp
    Systems/System.inl
    Components/TagComponent.hpp
    Meta/ItemHelper.hpp
    Meta/ComponentHelper.hpp
)

target_sources(RkECS PRIVATE
    Archetype.cpp
    Entity.cpp
    Systems/SystemEventHandler.cpp
    Universe.cpp
    Range.cpp
    Components/TagComponent.cpp
    Systems/System.cpp
)

target_link_libraries(RkECS PUBLIC RkCore)
