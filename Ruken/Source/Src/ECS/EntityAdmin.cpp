/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "ECS/EntityAdmin.hpp"
#include "Core/ServiceProvider.hpp"

USING_RUKEN_NAMESPACE

RkVoid EntityAdmin::BuildUpdatePlan() noexcept
{
    m_update_plan.ResetPlan();

    for (auto& system: m_systems)
    {
        m_update_plan.AddInstruction([&system] { system->OnUpdate(); });
        m_update_plan.EndInstructionPack();
    }
}

EntityAdmin::EntityAdmin(ServiceProvider& in_service_provider) noexcept:
    Service     {in_service_provider},
    m_scheduler {m_service_provider.LocateService<Scheduler>()}
{
    // The entity admin requires a scheduler to be able to work
    if (!m_scheduler)
        SignalServiceInitializationFailure("The entity admin requires a scheduler to be able to work, updates are asynchronous");
}

RkVoid EntityAdmin::StartSimulation() noexcept
{
    // Simulation start is synchronous for now
    for (auto& system: m_systems)
        system->OnStart();
}

RkVoid EntityAdmin::UpdateSimulation() noexcept
{
    m_update_plan.ExecutePlanAsynchronously(*m_scheduler);
}

RkVoid EntityAdmin::EndSimulation() noexcept
{
    // Simulation end is synchronous for now
    for (auto& system: m_systems)
        system->OnEnd();
}
