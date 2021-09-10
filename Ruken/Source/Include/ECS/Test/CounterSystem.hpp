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
#include "ECS/EventHandler.hpp"
#include "ECS/ComponentView.hpp"
#include "ECS/Test/CounterComponent.hpp"

USING_RUKEN_NAMESPACE

struct CounterSystem final: public System
{
    CounterSystem(EntityAdmin& in_admin) : System(in_admin)
    {
        // Setup of the different event handlers
        SetupEventHandler<StartHandler >();
        SetupEventHandler<UpdateHandler>();
    }

    #pragma region Methods

    /**
     * \brief Start event dispatcher
     *
     * Called once at the start of the simulation
     * This method could be called multiple times for the same
     * instance if the simulation is restarted without reloading the whole ECS 
     */
    struct StartHandler final: StartEventHandler<CounterComponent>
    {
        using CountView = MakeView<CounterComponent::CountField>;

        // Actual event dispatcher method
        RkVoid Execute() noexcept override
        {
            RkSize value {0ULL};

            for (auto&& group : m_groups)
            {
                for (CountView view = group.GetView<CountView>(); view.FindNextEntity();)
                    view.Fetch<CounterComponent::CountField>() = value++;
            }

            std::cout << "updated " << value << " entities\n"; 
        }
    };

    /**
     * \brief Update event dispatcher
     *
     * This event is called once per frame while the scene is active
     * The "OnStart" event is always called before the first call to this event
     * and the "OnEnd" event is always fired after the last call to this event
     */
    struct UpdateHandler final: UpdateEventHandler<CounterComponent>
    {
        using CountView = MakeView<CounterComponent::CountField>;

        // Actual event dispatcher method
        RkVoid Execute() noexcept override
        {
            for (auto&& group : m_groups)
            for (CountView view = group.GetView<CountView>(); view.FindNextEntity();)
                view.Fetch<CounterComponent::CountField>()++;
        }
    };

    #pragma endregion
};
