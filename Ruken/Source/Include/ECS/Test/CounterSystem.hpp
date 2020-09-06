/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
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

#pragma once

#include "ECS/System.hpp"

#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterComponent.hpp"
#include "ECS/Test/TestExclusiveComponent.hpp"

USING_RUKEN_NAMESPACE

struct CounterSystem final : System<CounterComponent, TestTagComponent>
{
    using System<CounterComponent, TestTagComponent>::System;

    using StartView  = CounterComponent::Item::MakeView<Count>;
    using UpdateView = CounterComponent::Item::MakeView<Count const> const;

    /**
     * \brief Called once at the start of a simulation
     * \note This method could be called multiple times for the same
     *       instance if the simulation is restated without reloading the whole ECS 
     */
    RkVoid OnStart() noexcept override
    {
        RkSize count = 0;

        TestExclusiveComponent* test = m_admin.GetExclusiveComponent<TestExclusiveComponent>();

        test->data1 = 1;
        test->data2 = 2;
        test->data3 = 3;

        // Iterating over every entity
        for (auto && group: m_groups)
        for (EntityID entity; entity < group.GetReferencedArchetype().EntitiesCount(); ++entity)
        {
            // Creating views
            StartView view = group.GetComponent<CounterComponent>().GetItemView<StartView>(entity);

            // Setting the "CounterComponent::Count" variable
            view.Fetch<Count>() = count++;
        }
    }

    /**
     * \brief Called every frame
     * \param in_time_step Time passed in seconds since the last frame
     */
    RkVoid OnUpdate([[maybe_unused]] RkFloat in_time_step) noexcept override
    {
        RkSize total = 0;

        // Iterating over every entity
        for (auto && group: m_groups)
        for (EntityID entity; entity < group.GetReferencedArchetype().EntitiesCount(); ++entity)
        {
            // Creating views
            UpdateView view = group.GetComponent<CounterComponent>().GetItemView<UpdateView>(entity);

            // Reading the "CounterComponent::Count" variable
            total += view.Fetch<Count>();
        }
    }
};
