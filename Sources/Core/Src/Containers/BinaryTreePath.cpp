#include "Containers/BinaryTreePath.hpp"
#include "Meta/Assert.hpp"

USING_RUKEN_NAMESPACE

RkUint64 BinaryTreePath::GetNodeCount(RkUint64 const in_start_depth) const noexcept
{
	RkUint64 value {};

	for (RkUint64 index = in_start_depth; index <= max_depth; ++index)
		value |= 1ULL << (max_depth - index);

	return value;
}

RkUint64 BinaryTreePath::GetIndex() const noexcept
{
	auto recursive_get_index = [&](this const auto& in_self, RkUint64 const in_current_depth = 0) -> RkUint64 {

		if (depth == in_current_depth)
			return 0;

		const RkUint64 depth_mask   {1ULL << in_current_depth};
		const RkBool   is_bit_set   {(path & depth_mask) == depth_mask};
		const RkUint64 contribution {1 + GetNodeCount(in_current_depth + 1) * is_bit_set};

		return contribution + in_self(in_current_depth + 1);
	};

	return recursive_get_index();
}

RkBool BinaryTreePath::IsParentOf(BinaryTreePath const& in_path) const noexcept
{
	for (RkUint64 i = 0ULL; i < depth; ++i)
        if ((in_path.path & 1ULL << i) != (path & 1ULL << i))
        	return false;

	return true;
}

void BinaryTreePath::Parent() noexcept
{
	RUKEN_ASSERT(depth != 0, ""); depth--;
}

void BinaryTreePath::LeftChild() noexcept
{
	path &= ~(1ULL << depth); depth++; RUKEN_ASSERT(depth <= max_depth, "");
}

void BinaryTreePath::RightChild() noexcept
{
	path |= 1ULL << depth ; depth++; RUKEN_ASSERT(depth <= max_depth, "");
}

void BinaryTreePath::Sibling() noexcept
{
	path ^= 1ULL << (depth - 1);
}

void BinaryTreePath::ChildFromBias(RkUint64 const in_bias) noexcept
{
	path |= in_bias & 1ULL << depth; depth++;
	RUKEN_ASSERT(depth <= max_depth, "");
}

RkBool BinaryTreePath::IsRoot() const noexcept { return depth == 0; }
RkBool BinaryTreePath::IsLeaf() const noexcept { return depth == max_depth; }
