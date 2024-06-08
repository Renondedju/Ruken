#pragma once

#include "Core/ExecutiveSystem/CPU/Awaitables/Primitives/CountDownLatch.hpp"

#include "ECS/System.hpp"
#include "ECS/EventHandler.hpp"
#include "ECS/Test/CounterComponent.hpp"

USING_RUKEN_NAMESPACE

inline CPUDynamicTask<> WhenAll(std::vector<CPUDynamicTask<>> const& in_jobs)
{
    CountDownLatch               latch         {in_jobs.size()};
    std::vector<CPUContinuation> continuations {in_jobs.size()};

    for (RkSize index = 0; index < in_jobs.size(); ++index)
    {
        // Creating a job and keeping a reference onto it until the attachment
        // process has been done to avoid it being deleted in the meanwhile.

        // Instantiating the job and setup the continuation callback
        continuations[index].Setup(latch, in_jobs[index]);

        // Trying to attach the continuation and if the process failed, that means the event we
        // were looking to await has already been completed. Thus we need to decrement manually the latch by one.
        if (!continuations[index].TryAttach())
            latch.CountDown();
    }

    co_await latch;
}

struct CounterSystem final: public System
{
    CounterSystem(EntityAdmin& in_admin) : System(in_admin)
    {
        // Setup of the different event handlers
        SetupEventHandler<StartHandler>();
    }

    #pragma region Methods

    /**
     * \brief Start event dispatcher
     *
     * Called once at the start of the simulation
     * This method could be called multiple times for the same
     * instance if the simulation is restarted without reloading the whole ECS 
     */
    struct StartHandler final: EventHandler<EEventName::OnStart, CounterComponent::CountField>
    {
	    static CPUDynamicTask<RkVoid> ProcessChunk(LinkedChunkListNode<unsigned long long>& in_node) noexcept
        {
            for (auto& data: in_node.data)
                data++;

            co_return;
        }

        CPUDynamicTask<RkVoid> Execute() noexcept override
        {
            RkSize task_count {0};
            for (auto const& archetype: m_archetypes)
            {
                task_count += archetype.get()
                    .GetComponent     <CounterComponent>            ()
                    .GetFieldContainer<CounterComponent::CountField>()
                    .GetSize();
            }

            std::vector<CPUDynamicTask<RkVoid>> tasks {task_count};
            RkSize index {0};
            for (auto const& archetype: m_archetypes)
            {
                auto& container = archetype.get()
                        .GetComponent     <CounterComponent>            ()
                        .GetFieldContainer<CounterComponent::CountField>();

                using Container = std::remove_reference_t<decltype(container)>;
                for (Container::Node* current_node = container.GetHead(); current_node != nullptr; current_node = current_node->next_node)
                    tasks[index++] = ProcessChunk(*current_node);
            }

            co_await WhenAll(tasks);
        }
    };

    #pragma endregion
};
