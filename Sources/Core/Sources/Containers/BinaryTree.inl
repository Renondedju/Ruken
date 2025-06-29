#pragma once

#include "Containers/BinaryTree.hpp"

BEGIN_RUKEN_NAMESPACE

template<typename TData>
BinaryTree<TData>::BinaryTree(RkUint64 const in_capacity) noexcept:
	leaf_count {static_cast<RkUint64>(std::pow(2, std::ceil(std::log(in_capacity) / std::log(2))))},
	max_depth  {static_cast<RkUint64>(std::log2(leaf_count))},
	node_count {GetNodeCount()},
	nodes      {new TData[node_count]}
{}

template<typename TData>
RkUint64 BinaryTree<TData>::GetNodeCount() const
{
	RkUint64 value {};

	for (RkUint64 index = 0; index <= max_depth; ++index)
		value |= 1ULL << index;

	return value;
}

template<typename TData>
BinaryTreePath BinaryTree<TData>::Root() const noexcept
{
	return BinaryTreePath { .max_depth = max_depth };
}

END_RUKEN_NAMESPACE