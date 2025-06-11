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

	// Selection methods
    void Parent       () noexcept;
    void LeftChild    () noexcept;
    void RightChild   () noexcept;
    void Sibling      () noexcept;
    void ChildFromBias(RkUint64 in_bias) noexcept;

    RkBool IsRoot() const noexcept;
    RkBool IsLeaf() const noexcept;
};

END_RUKEN_NAMESPACE
