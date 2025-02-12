#pragma once

#include "ECS/System.hpp"
#include "ECS/EventHandler.hpp"
#include "ECS/Test/CounterComponent.hpp"

#include "ExecutiveSystem/CPU/Awaitables/Primitives/WhenAll.hpp"

USING_RUKEN_NAMESPACE

struct CounterSystem final: System
{
    explicit CounterSystem(EntityAdmin& in_admin) : System(in_admin)
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
	    static CPUDynamicTask<RkVoid> ProcessChunk(LinkedChunkListNode<RkSize>& in_node) noexcept
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
            RkSize                              index {0};
            for (auto const& archetype: m_archetypes)
            {
                auto& container = archetype.get()
                        .GetComponent     <CounterComponent>            ()
                        .GetFieldContainer<CounterComponent::CountField>();

                using Container = std::remove_reference_t<decltype(container)>;
                for (Container::Node* current_node = container.GetHead(); current_node != nullptr; current_node = current_node->next_node)
                    tasks[index++] = ProcessChunk(*current_node);
            }

	        auto a = WhenAll(tasks);
	        co_await a;
        }
    };

    #pragma endregion
};
