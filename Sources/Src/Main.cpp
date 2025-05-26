#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"

#include "JobSystem/JobSystem.hpp"
#include "JobSystem/Queues/QueueHandle.hpp"
#include "JobSystem/Awaitables/Tasks/Task.hpp"
#include "JobSystem/Awaitables/Primitives/SharedMutex.hpp"

#include "Debug/Logging/Logger.hpp"
#include "Debug/Logging/Handlers/DebugHandler.hpp"
#include "Debug/Logging/Handlers/ConsoleHandler.hpp"

#include <tracy/Tracy.hpp>

struct MainQueue : QueueHandle<MainQueue, 2048>
{};

#define RUKEN_IO_QUEUE MainQueue

#include "IO/File.hpp"
#include "IO/IOJobQueue.hpp"

USING_RUKEN_NAMESPACE

struct AsyncLoop
{
    const char* name;
    EntityAdmin scene;

    Task<MainQueue> Setup()
    {
        scene.CreateSystem<CounterSystem>();
        for (int i = 0; i < 10'000'000; i++)
            scene.CreateEntity<CounterComponent>();

        co_await scene.ExecuteEvent(EEventName::OnStart);
    }

    [[nodiscard]]
    Task<MainQueue> Run() noexcept
    {
        // When the awaited primitive is signaled (in this case when the task is done),
        // the Run() coroutine is then scheduled back into the MainQueue, waiting to be picked up
        // by the first available thread.

        // Main loop
        for (int i = 0; i < 2000; ++i)
        {
            co_await scene.ExecuteEvent(EEventName::OnStart);
            File::on_io_pull.Signal();

            FrameMark;
        }

        co_await scene.ExecuteEvent(EEventName::OnEnd);
    }
};

Task<MainQueue> Read(SharedMutex<RkInt64>& in_mutex) {
    auto access = co_await in_mutex.AsyncRead();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

Task<MainQueue> Write(SharedMutex<RkInt64>& in_mutex) {
    auto access = co_await in_mutex.AsyncWrite();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    (*access)++;
}

Task<MainQueue> Pull(std::stop_source& in_stop_source)
{
    if (in_stop_source.stop_requested())
        co_return;

    File::on_io_pull.Signal();
    Pull(in_stop_source);

    co_return;
}

/**
 * Asynchronous main.
 * @param in_stop_source Stop token. Used to prompt the main thread to go out of scope.
 * @param in_service_provider Service Provider.
 */
Task<MainQueue> AsyncMain(std::stop_source& in_stop_source, ServiceProvider& in_service_provider)
{
    AsyncLoop loop {"Loop", EntityAdmin {in_service_provider}};

    File const file("C:\\Users\\Basile\\Downloads\\Sicion- KUSS - The Journey EP -MLKL041.zip");
    File const other("D:\\screen_space_shadows.zip");

    co_await loop.Setup();

    co_await WhenAllVariadic(
        //file .ReadEverything(),
        //other.ReadEverything(),
        loop.Run()
    );

    in_stop_source.request_stop();

    co_return;
}

struct BinaryTreePath
{
    RkUint64       path      {};   // Binary path of the node. bit 0 = left child, bit 1 = right child
    RkUint64       depth     {};  // Depth of the path.
    const RkUint64 max_depth {}; // Max depth of the

    /**
      * Computes the amount of nodes required to store a tree of depth TDepth,
      * excluding layers before in_start_depth.
      *
      * @param in_start_depth Start depth of the count.
      * @return Node count.
      */
    constexpr RkUint64 GetNodeCount(RkUint64 const in_start_depth) const noexcept
    {
        RkUint64 value {};

        for (RkUint64 index = in_start_depth; index <= max_depth; ++index)
            value |= 1 << max_depth - index;

        return value;
    }

    /**
     * Returns the address of a node.
     * @return Index of the node within the nodes array.
     */
    constexpr RkUint64 GetIndex() const noexcept
    {
        auto recursive_get_index = [&](this const auto& in_self, RkUint64 const in_current_depth = 0) {

            if (depth == in_current_depth)
                return 0;

            const RkUint64 depth_mask   {static_cast<RkUint64>(1 << in_current_depth)};
            const RkBool   is_bit_set   {(path & depth_mask) == depth_mask};
            const RkUint64 contribution {1 + GetNodeCount(in_current_depth + 1) * is_bit_set};

            return contribution + in_self(in_current_depth + 1);
        };

        return recursive_get_index();
    }

    constexpr void Parent    () noexcept {                        depth--; RUKEN_ASSERT(depth >= 0);         }
    constexpr void LeftChild () noexcept { path &= ~(1 << depth); depth++; RUKEN_ASSERT(depth <= max_depth); }
    constexpr void RightChild() noexcept { path |=   1 << depth ; depth++; RUKEN_ASSERT(depth <= max_depth); }

    constexpr RkBool IsRoot() const noexcept { return depth == 0; }
    constexpr RkBool IsLeaf() const noexcept { return depth == max_depth; }
};

/**
 * A contiguous binary tree.
 * @tparam TDepth Depth of the tree.
 * @tparam TData Data type.
 */
template <RkUint64 TDepth, typename TData>
struct BinaryTree
{
    #pragma region Methods

    /**
     * Computes the amount of nodes required to store a tree of depth TDepth.
     * @return Node count.
     */
    static constexpr RkUint64 GetNodeCount()
    {
        RkUint64 value {};

        for (RkUint64 index = 0; index <= TDepth; ++index)
            value |= 1 << index;

        return value;
    }

    /// @returns the root node.
    static constexpr BinaryTreePath Root() noexcept
    { return BinaryTreePath { .max_depth = TDepth }; }

    #pragma endregion

    static constexpr RkUint64 depth      = TDepth;
    static constexpr RkUint64 leaf_count = 1 << depth; // == pow(2, depth)
    static constexpr RkUint64 node_count = GetNodeCount();

    /**
     * Memory Layout example for a tree depth of 3:
     * Root node is A0. Leafs are *3.
     * std::array<>{ | | | | | | | | | | | | | | | }
     *   depth = 3 - | | | A3B3| C3D3| | E3F3| G3H3
     *           2 - | | A2    C2    | E2    G2
     *           1 - | A1            E1
     *           0 - A0
     * .
     *           A0
     *         /    \
     *       A1      E1
     *      /  \    /  \
     *     A2  C2  E2  G2
     */
    std::array<TData, node_count> nodes {};
};

template <RkUint64 TDepth>
struct ConcurrencyTree
{
    BinaryTree<TDepth, std::atomic_uint64_t> tree                {};
    BinaryTree<TDepth, RkUint8>              maximum_concurrency {};

    void IncrementOptimal(BinaryTreePath in_leaf)
    {
        for (int depth = 0; depth < TDepth; ++depth)
        {
            tree.nodes[in_leaf.GetIndex()].fetch_add(s_one_optimal.value, std::memory_order_acq_rel);
            in_leaf.Parent();
        }
    }

    void DecrementOptimal(BinaryTreePath in_leaf)
    {
        for (int depth = 0; depth < TDepth; ++depth)
        {
            tree.nodes[in_leaf.GetIndex()].fetch_sub(s_one_optimal.value, std::memory_order_acq_rel);
            in_leaf.Parent();
        }
    }

    void FindLeaf(BinaryTreePath in_bias)
    {
        BinaryTreePath path {tree.Root()};

        for (int depth = 0; depth < TDepth; ++depth)
        {
            RkUint64           index   {path.GetIndex()};
            ConcurrencyCounter current {tree.nodes[index].load(std::memory_order_acquire)};
            ConcurrencyCounter desired {};

            do
            {
                // Decide ?
                decide(current, )

                desired.value = current.value + s_one_current.value;

            } while (!tree.nodes[index].compare_exchange_weak(current.value, desired, std::memory_order_acq_rel));

            if (true) path.LeftChild ();
            else      path.RightChild();
        }
    }
};

/**
 * Initializes services and waits for the async main function to request a stop.
 * @param in_argc Argument count
 * @param in_argv Argument values
 * @return Error code
 */
int main(int in_argc, char* in_argv[])
{
    // Setup logging
    ConsoleHandler console_handler {};
    DebugHandler   debug_handler   {{}};

    std::initializer_list<LogHandler*> handlers { &console_handler, &debug_handler };

    // Initializing services and core systems
    ServiceProvider services   {"Root"};
    Logger*         logger     {services.ProvideService<Logger>(handlers)};
    JobSystem*      job_system {services.ProvideService<JobSystem>()};

    std::stop_source stop_source {};

    // Pushing async main to the MainQueue
    AsyncMain(stop_source, services);

    // Starting workers
    job_system->RegisterQueue (MainQueue::instance);
    job_system->StartWorkers  ();
    job_system->CallerAsWorker(stop_source.get_token());

    return 0;
}