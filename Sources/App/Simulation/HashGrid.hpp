#pragma once

#include "Core/Types/FundamentalTypes.hpp"
#include "Core/Maths/Vector/DistanceVector.hpp"

#include <vector>
#include <array>

BEGIN_RUKEN_NAMESPACE

struct HashGrid
{
	explicit HashGrid(RkSize const in_size):
		cell_size {1_m},
		hash_map  {in_size}
	{}

	/// @brief Used to implement a hash grid (spatial partition)
	struct HashIndex
	{
		RkUint32 hash		  {};
		RkUint32 entity_index {};
	};

	inline static std::array s_3d_neighbors {
		Constants<Vector3m>::zero,

		// Direct neighbors
		Vector3m( 1.0_m,  0.0_m,  0.0_m),
		Vector3m(-1.0_m,  0.0_m,  0.0_m),
		Vector3m( 0.0_m,  1.0_m,  0.0_m),
		Vector3m( 0.0_m, -1.0_m,  0.0_m),
		Vector3m( 0.0_m,  0.0_m,  1.0_m),
		Vector3m( 0.0_m,  0.0_m, -1.0_m),

		// Diagonals
		Vector3m( 1.0_m, -1.0_m, -1.0_m),
		Vector3m( 1.0_m, -1.0_m,  1.0_m),
		Vector3m(-1.0_m, -1.0_m,  1.0_m),
		Vector3m(-1.0_m, -1.0_m, -1.0_m),
		Vector3m( 1.0_m,  1.0_m, -1.0_m),
		Vector3m( 1.0_m,  1.0_m,  1.0_m),
		Vector3m(-1.0_m,  1.0_m,  1.0_m),
		Vector3m(-1.0_m,  1.0_m, -1.0_m),
		Vector3m( 1.0_m,  0.0_m, -1.0_m),
		Vector3m( 1.0_m,  0.0_m,  1.0_m),
		Vector3m(-1.0_m,  0.0_m,  1.0_m),
		Vector3m(-1.0_m,  0.0_m, -1.0_m)
	};

	Meters				   cell_size;
	std::vector<HashIndex> hash_map;

	/// @brief Creates a 3D/2D hash for the passed position
	RkUint32 MakeHash(Vector3m in_position) const noexcept;
	RkUint32 MakeHash(Vector2m in_position) const noexcept;

	RkVoid SetHashIndex(Vector3m in_position, RkUint32 in_entity_index) noexcept;
	RkVoid SetHashIndex(Vector2m in_position, RkUint32 in_entity_index) noexcept;

	/// @brief Sorts hashes to prepare for binary search
	RkVoid SortHashes();

	/**
	 * Binary search though the sorted hashes list and returns the first occurrence of the passed hash.
	 * @param in_hash Hash to look for.
	 * @return Index of the first occurrence of the passed hash. May be null.
	 */
	std::optional<RkSize> SearchFirst(RkUint32 in_hash) const noexcept;
};

END_RUKEN_NAMESPACE