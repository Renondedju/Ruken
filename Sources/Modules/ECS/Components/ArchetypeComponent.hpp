#pragma once

#include "ECS/Components/Component.hpp"
#include "Core/JobSystem/Awaitables/SyncTask/SyncTask.hpp"

BEGIN_RUKEN_NAMESPACE

struct ArchetypeComponent: Component
{
	#pragma region Lifetime

	ArchetypeComponent()									 = default;
	ArchetypeComponent& operator=(ArchetypeComponent const&) = default;
	ArchetypeComponent& operator=(ArchetypeComponent&&     ) = default;
	ArchetypeComponent		     (ArchetypeComponent const&) = default;
	ArchetypeComponent		     (ArchetypeComponent&&     ) = default;
	virtual ~ArchetypeComponent()							 = default;

	#pragma endregion

	virtual SyncTask<> CreateEntities(RkSize in_count = 1) noexcept = 0;
	//virtual RkVoid DeleteEntities(RkSize in_index)	   noexcept = 0;
};

template <typename TType>
using ArchetypeComponentType = std::is_base_of<ArchetypeComponent, TType>;

template <typename TType>
concept IsArchetypeComponent = ArchetypeComponentType<TType>::value;

END_RUKEN_NAMESPACE