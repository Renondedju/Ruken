#pragma once

#include "Core/Containers/BinaryTreePath.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * A contiguous binary tree.
 * @tparam TData Data type.
 */
template <typename TData>
struct BinaryTree
{
	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_capacity Minimum amount of leafs contained in the tree.
	 */
	explicit BinaryTree(RkUint64 in_capacity) noexcept;
	 BinaryTree           (BinaryTree const&) = delete;
	 BinaryTree           (BinaryTree&&)      = delete;
	 BinaryTree& operator=(BinaryTree const&) = delete;
	 BinaryTree& operator=(BinaryTree&&)      = delete;
	~BinaryTree()							  = default;

	#pragma endregion

	#pragma region Methods

	/**
	 * Computes the amount of nodes required to store a tree of depth TDepth.
	 * @return Node count.
	 */
	RkUint64 GetNodeCount() const;

	/// @returns the root node.
	BinaryTreePath Root() const noexcept;

	#pragma endregion

	#pragma region Members

	RkUint64 				 const leaf_count {};
	RkUint64 				 const max_depth  {};
	RkUint64 				 const node_count {};
	std::unique_ptr<TData[]> const nodes	  {};
	//TData*   const nodes      {};
	/* Memory Layout example for a tree depth of 3:
	 * Root node is A0. Leafs are *3.
	 * { | | | | | | | | | | | | | | | }
	 *   | | | A3B3| C3D3| | E3F3| G3H3
	 *   | | A2    C2    | E2    G2
	 *   | A1            E1
	 *   A0
	 */

	#pragma endregion
};

END_RUKEN_NAMESPACE

#include "BinaryTree.inl"
