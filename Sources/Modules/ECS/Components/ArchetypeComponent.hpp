#pragma once

#include "Components/Component.hpp"

BEGIN_RUKEN_NAMESPACE

struct ArchetypeComponent: Component
{
	#pragma region Lifetime

	ArchetypeComponent()								     = default;
	ArchetypeComponent& operator=(ArchetypeComponent const&) = default;
	ArchetypeComponent& operator=(ArchetypeComponent&&     ) = default;
	ArchetypeComponent           (ArchetypeComponent const&) = default;
	ArchetypeComponent           (ArchetypeComponent&&     ) = default;
	virtual ~ArchetypeComponent()						     = default;

	#pragma endregion

	/**
	 * @brief Ensures that the component has enough storage space for a given amount of entities
	 *        If this is not the case, containers will be allocated
	 * @param in_size Size to ensure
	 * @return Minimum number of elements allocated by one of the containers in the layout
	 *         This can be useful to avoid having to call back this function when no new allocation is needed
	 */
	[[nodiscard]]
	virtual RkSize EnsureStorageSpace(RkSize in_size) noexcept = 0;
};

template <typename TType>
concept IsArchetypeComponent = std::is_base_of_v<TType, ArchetypeComponent>;

END_RUKEN_NAMESPACE