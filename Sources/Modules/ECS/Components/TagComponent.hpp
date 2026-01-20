#pragma once

#include "ECS/Components/ArchetypeComponent.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief A tag component does not hold any data, its only purpose
///		   is to differentiate 2 entities from each other by "tagging" it.
struct TagComponent: ArchetypeComponent
{
    #pragma region Lifetime

    TagComponent()								 = default;
	TagComponent& operator=(TagComponent const&) = default;
	TagComponent& operator=(TagComponent&&     ) = default;
    TagComponent		   (TagComponent const&) = default;
    TagComponent		   (TagComponent&&     ) = default;
    ~TagComponent() override				     = default;

    #pragma endregion

	SyncTask<> CreateEntities(RkSize in_count = 1) noexcept override;
};

template<typename TType>
using TagComponentType = std::is_base_of<TagComponent, TType>;

END_RUKEN_NAMESPACE