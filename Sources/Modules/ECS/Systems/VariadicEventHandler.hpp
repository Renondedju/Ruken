#pragma once

#include "ECS/ECSTask.hpp"
#include "ECS/Universe.hpp"
#include "ECS/Systems/SystemEventHandler.hpp"
#include "ECS/Components/EntityComponent.hpp"
#include "ECS/Components/UniverseComponent.hpp"

#include "Core/Meta/TupleIndex.hpp"
#include "Core/Meta/TupleSubset.hpp"
#include "Core/Meta/TupleTransform.hpp"
#include "Core/Meta/TupleRemoveCvRef.hpp"

BEGIN_RUKEN_NAMESPACE

// TODO: C++26 template for should allow a nicer syntax
template <IsComponent... TComponents>
struct VariadicEventHandler: SystemEventHandler
{
	#pragma region Lifetime

	explicit VariadicEventHandler() noexcept:
		SystemEventHandler {[](ComponentFingerprint const& in_universe, ComponentFingerprint const& in_archetype) {
			auto const query_fingerprint {ComponentFingerprint::CreateFingerPrintFrom<TComponents...>()};
			return (in_universe + in_archetype).HasAll(query_fingerprint);
		}}
	{}

	VariadicEventHandler		   (VariadicEventHandler const&) = default;
	VariadicEventHandler		   (VariadicEventHandler&&)      = default;
	VariadicEventHandler& operator=(VariadicEventHandler const&) = default;
	VariadicEventHandler& operator=(VariadicEventHandler&&)      = default;
	~VariadicEventHandler() override							 = default;

	#pragma endregion

	#pragma region Deduction Helpers

	// Type deduction helpers
	template <IsEntityComponent TComponent> struct EntityIteratorTransformer
	{ using TType = decltype(std::declval<TComponent>().storage.begin()); };

	template <typename TIterator> struct EntityAwaitableTransformer
	{ using TType = decltype(std::declval<TIterator >()->AsyncAccess()); };

	template <IsUniverseComponent TComponent> struct UniverseAwaitersTransformer
	{ using TType = decltype(std::declval<TComponent>().data.AsyncAccess()); };

	template <typename TAwaitable> struct ComponentAccessTransformer
	{ using TType = AwaitableTraits<TAwaitable>::AwaitResult; };

	#pragma endregion

	#pragma region Type Helpers

	using UniverseComponents     = TupleSubset<UniverseComponentType, TComponents...>::Type;
	using UniverseAwaitables     = TupleTransformT<UniverseAwaitersTransformer, UniverseComponents>;
	using EntityComponents       = TupleSubset<EntityComponentType  , TComponents...>::Type;
	using EntityStorageIterators = TupleTransformT<EntityIteratorTransformer, EntityComponents>;
	using EntityAwaitables       = TupleTransformT<EntityAwaitableTransformer, EntityStorageIterators>;
	using AddressableComponents  = decltype(std::tuple_cat(std::declval<UniverseComponents>(), std::declval<EntityComponents>()));
	using AddressableAwaitables  = decltype(std::tuple_cat(std::declval<UniverseAwaitables>(), std::declval<EntityAwaitables>()));
	using ComponentAccess		 = TupleTransformT<ComponentAccessTransformer, AddressableAwaitables>;

	#pragma endregion

	/// @brief Executes the actual content of the handler
	virtual RkVoid Execute(RkSize in_chunk_index, RkSize in_chunk_size, ComponentAccess& in_access_tuple) const noexcept = 0;

	template <IsComponent TComponent>
	static auto& Fetch(ComponentAccess& in_access_tuple, RkSize in_index) noexcept
	{
		static constexpr auto index {TupleIndex<
			std::remove_cv_t<TComponent>,
			typename TupleRemoveCvRef<AddressableComponents>::Type
		>::value};

		return (*std::get<index>(in_access_tuple))[in_index];
	}

	private:

		#pragma region Methods

		/// @brief Schedules execution of the handler.
		SyncTask<> ScheduleExecution(Universe& in_universe, Archetype& in_archetype) noexcept override;

		/**
		 * Acquires the component locks and dispatches the actual handler logic.
		 * @param in_chunk_index Chunk index being processed
		 * @param in_component_access Components access.
		 * @return Sync task handle.
		 */
		ECSTask<RkVoid> ProcessChunk(RkSize in_chunk_index, SyncTask<ComponentAccess> in_component_access) noexcept;

		// Various helpers hiding ugly variadic logic
		EntityStorageIterators GetEntityStorageIterators     (Archetype& in_archetype);
		EntityAwaitables	   GetEntityStorageAwaiters      (EntityStorageIterators& in_iterators);
		UniverseAwaitables     GetUniverseComponentAwaitables(Universe& in_universe);

		#pragma endregion
};

END_RUKEN_NAMESPACE

#include "VariadicEventHandler.inl"