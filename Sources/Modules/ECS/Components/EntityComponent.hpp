#pragma once

#include "ECS/Components/ArchetypeComponent.hpp"
#include "Core/Containers/LinkedChunkList.hpp"
#include "Core/Types/Concepts/NonVoid.hpp"

BEGIN_RUKEN_NAMESPACE

template <NonVoid TData>
struct EntityComponent: ArchetypeComponent
{
    #pragma region Lifetime

    EntityComponent()								   = default;
    EntityComponent& operator=(EntityComponent const&) = default;
    EntityComponent& operator=(EntityComponent&&     ) = default;
    EntityComponent           (EntityComponent const&) = default;
    EntityComponent           (EntityComponent&&     ) = default;
    ~EntityComponent() override						   = default;

    #pragma endregion

    /// @copydoc ArchetypeComponent::EnsureStorageSpace
    [[nodiscard]] RkSize EnsureStorageSpace(RkSize in_size) noexcept override;

	/// @brief Data chunks
	LinkedChunkList<TData> storage {};
};

#include "EntityComponent.inl"

END_RUKEN_NAMESPACE
