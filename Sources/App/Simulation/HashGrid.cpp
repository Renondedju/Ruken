#include "HashGrid.hpp"

#include "Core/Maths/Utility.hpp"

USING_RUKEN_NAMESPACE

RkUint32 HashGrid::MakeHash(Vector3m in_position) const noexcept
{
	return  static_cast<RkUint32>(Floor(static_cast<RkFloat>(in_position.x() / cell_size))) * 73856093ui32 ^
			static_cast<RkUint32>(Floor(static_cast<RkFloat>(in_position.y() / cell_size))) * 19349663ui32 ^
			static_cast<RkUint32>(Floor(static_cast<RkFloat>(in_position.z() / cell_size))) * 83492791ui32;
}

RkUint32 HashGrid::MakeHash(Vector2m in_position) const noexcept
{
	return  static_cast<RkUint32>(Floor(static_cast<RkFloat>(in_position.x() / cell_size))) * 73856093ui32 ^
			static_cast<RkUint32>(Floor(static_cast<RkFloat>(in_position.y() / cell_size))) * 19349663ui32;
}

RkVoid HashGrid::SetHashIndex(Vector3m const in_position, RkUint32 const in_entity_index) noexcept
{
	hash_map[in_entity_index] = HashIndex {
		.hash		  = MakeHash(in_position),
		.entity_index = in_entity_index
	};
}

RkVoid HashGrid::SetHashIndex(Vector2m const in_position, RkUint32 const in_entity_index) noexcept
{
	hash_map[in_entity_index] = HashIndex {
		.hash         = MakeHash(in_position),
		.entity_index = in_entity_index
	};
}

RkVoid HashGrid::SortHashes()
{
	std::ranges::sort(hash_map, [](HashIndex const& in_lhs, HashIndex const& in_rhs) {
		return in_lhs.hash > in_rhs.hash;
	});
}

std::optional<RkSize> HashGrid::SearchFirst(RkUint32 const in_hash) const noexcept
{
	RkInt64 			  left   {0};
	RkInt64 			  right  {static_cast<RkInt64>(hash_map.size()) - 1};
	std::optional<RkSize> result {std::nullopt};

	while (left <= right)
	{
		RkInt64  const mid      {(left + right) / 2};
		RkUint32 const mid_hash {hash_map[mid].hash};

		if (mid_hash == in_hash) {
			result = mid;
			right  = mid - 1uz;
		} else if (mid_hash < in_hash) {
			left   = mid + 1uz;
		} else
			right  = mid - 1uz;
	}

	return result;
}
