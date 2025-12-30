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

    /**
     * @note Since a tag component does not contain any data, this method does nothing
     * @copydoc ArchetypeComponent::EnsureStorageSpace
     */
    [[nodiscard]] RkSize EnsureStorageSpace(RkSize in_size) noexcept override;
};

END_RUKEN_NAMESPACE