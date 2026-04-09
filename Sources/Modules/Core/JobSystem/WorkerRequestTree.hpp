#pragma once

#include "Core/Containers/BinaryTree.hpp"

#include <condition_variable>
#include <tracy/Tracy.hpp>
#include <atomic>

BEGIN_RUKEN_NAMESPACE

/**
 * A synchronization structure used to prioritize tasks between a number of threads.
 */
struct WorkerRequestTree
{
    BinaryTree<std::atomic_uint64_t> tree;
    std::condition_variable_any      sleep_variable {};
    TracyLockableN(std::mutex      , sleep_mutex, "Worker sleep mutex");

    static inline thread_local RkUint64 fair_path {};

    /**
     * Constructor.
     * @param in_minimum_capacity
     */
    explicit WorkerRequestTree(RkUint64 const in_minimum_capacity) noexcept:
        tree {in_minimum_capacity}
    {}

    /**
     * Emits a request for the passed leaf
     * @param in_leaf Leaf to request something for.
     */
    RkVoid EmitRequest(BinaryTreePath in_leaf) noexcept
    {
        ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

        tree.nodes[in_leaf.GetIndex()].fetch_add(1, std::memory_order_acq_rel);

        for (RkUint64 depth {}; depth < tree.max_depth; ++depth)
        {
            in_leaf.Parent();
            tree.nodes[in_leaf.GetIndex()].fetch_add(1, std::memory_order_acq_rel);
        }

        sleep_variable.notify_one();
    }

    /**
     * Waits for and consumes or request based on a bias.
     * @param in_preferred_path Preferred path in the tree. If it does not directly lead to a leaf,
     *                          the rest of the traversal will be uniformly distributed over time.
     * @param in_stop_signal Stop signal. Used to cancel the wait.
     * @returns Path to the leaf that emitted the request we consumed.
     */
    BinaryTreePath WaitConsumeRequest(BinaryTreePath const& in_preferred_path, std::stop_token const& in_stop_signal) noexcept
    {
        ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

        BinaryTreePath path {tree.Root()};

        // Try to acquire root node and sleeping if that can't be done
        std::unique_lock lock {sleep_mutex};
        if (!sleep_variable.wait(lock, in_stop_signal, [&] { return TryConsumeRequest(path); }))
            return {};

        // Successfully consumed a request from the root node. That means we cannot
        // return anymore without going all the way down,
        // Otherwise we would leave the tree in an invalid state.

        for (RkUint64 depth {}; depth < tree.max_depth; ++depth)
        {
            // Use the preferred path up to its depth and fall-back to a fair path after that
            path.ChildFromBias(depth < in_preferred_path.depth ? in_preferred_path.path : GetFairPath(depth));

            while (!TryConsumeRequest(path))
                path.Sibling();
        }

        return path;
    }

    private:

        /**
         * Returns a fair bias path over time.
         * @param in_depth Depth to sample and switch the path at.
         * @return Bias.
         */
        static RkUint64 GetFairPath(RkUint64 const in_depth) noexcept
        { return fair_path ^= 1ULL << in_depth; }

        /**
         * Tries to consume a request from the passed path.
         * @param in_path Path to consume a request from.
         * @return True if the operation succeeded, false otherwise.
         */
        RkBool TryConsumeRequest(BinaryTreePath const& in_path) const noexcept
        {
            RkUint64 const index {in_path.GetIndex()};
            RkUint64       value {tree.nodes[index].load(std::memory_order_acquire)};

            do
            {
                if (value == 0ULL)
                    return false;
            } while (!tree.nodes[index].compare_exchange_weak(value, value - 1ULL, std::memory_order_acq_rel));

            return true;
        }
};

END_RUKEN_NAMESPACE