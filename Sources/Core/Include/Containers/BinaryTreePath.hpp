#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

struct BinaryTreePath
{
    RkUint64 path      {};   // Binary path of the node. bit 0 = left child, bit 1 = right child
    RkUint64 depth     {};  // Depth of the path.
    RkUint64 max_depth {}; // Max depth of the

    /**
      * Computes the amount of nodes required to store a tree of depth TDepth,
      * excluding layers before in_start_depth.
      *
      * @param in_start_depth Start depth of the count.
      * @return Node count.
      */
    RkUint64 GetNodeCount(RkUint64 in_start_depth) const noexcept;

    /**
     * Returns the address of a node.
     * @return Index of the node within the nodes array.
     */
    RkUint64 GetIndex() const noexcept;

    /**
     * Checks if the passed path is a child of this path.
     * @param in_path Path to check.
     * @return True if this path is a parent of in_path.
     */
    RkBool IsParentOf(BinaryTreePath const& in_path) const noexcept;

	// Selection methods
    RkVoid Parent       () noexcept;
    RkVoid LeftChild    () noexcept;
    RkVoid RightChild   () noexcept;
    RkVoid Sibling      () noexcept;
    RkVoid ChildFromBias(RkUint64 in_bias) noexcept;

    RkBool IsRoot() const noexcept;
    RkBool IsLeaf() const noexcept;
};

END_RUKEN_NAMESPACE
