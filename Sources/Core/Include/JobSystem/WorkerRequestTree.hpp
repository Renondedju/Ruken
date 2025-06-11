#pragma once

#include "Containers/BinaryTree.hpp"

#include <tracy/Tracy.hpp>
#include <atomic>

BEGIN_RUKEN_NAMESPACE

struct WorkerRequestTree
{
    BinaryTree<std::atomic_uint64_t> tree;
    std::condition_variable_any      sleep_variable {};
    std::mutex                       sleep_mutex    {};

    explicit WorkerRequestTree(RkUint64 const in_minimum_capacity) noexcept:
        tree {in_minimum_capacity}
    {}

    ~WorkerRequestTree()
    {
        sleep_variable.notify_all();
    }

    /**
     * Emits a request for the passed leaf
     * @param in_leaf Leaf to request something for.
     */
    RkVoid EmitRequest(BinaryTreePath in_leaf) noexcept
    {
        ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

        tree.nodes[in_leaf.GetIndex()].fetch_add(1, std::memory_order_acq_rel);

        for (int depth = 0; depth < tree.max_depth; ++depth)
        {
            in_leaf.Parent();
            tree.nodes[in_leaf.GetIndex()].fetch_add(1, std::memory_order_acq_rel);
        }

        sleep_variable.notify_one();
    }

    /**
     * Waits for and consumes or request based on a bias.
     * @param in_bias Preferred path in the tree.
     * @param in_stop_signal Stop signal.
     * @returns Path to the leaf that emitted the request we consumed.
     */
    BinaryTreePath WaitConsumeRequest(RkUint64 const in_bias, std::stop_token const& in_stop_signal) noexcept
    {
        ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_WORKER_ZONES));

        BinaryTreePath path {tree.Root()};

        // Try to acquire root node and sleeping if that can't be done
        std::unique_lock    lock {sleep_mutex};
        sleep_variable.wait(lock, [&] {
            return TryConsumeRequest(path) || in_stop_signal.stop_requested();
        });

        if (in_stop_signal.stop_requested())
            return tree.Root();

        // Successfully consumed a request from the root node. That means we cannot
        // return anymore without going all the way down,
        // Otherwise we would leave the tree in an invalid state.

        for (int depth = 0; depth < tree.max_depth; ++depth)
        {
            path.ChildFromBias(in_bias);

            while (!TryConsumeRequest(path))
                path.Sibling();
        }

        return path;
    }

    private:

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
                if (value == 0)
                    return false;
            } while (!tree.nodes[index].compare_exchange_weak(value, value - 1, std::memory_order_acq_rel));

            return true;
        }
};

END_RUKEN_NAMESPACE