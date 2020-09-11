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

#include "Core/Kernel.hpp"

#include "Utility/Benchmark.hpp"

#include "ECS/EntityAdmin.hpp"
#include "ECS/Test/CounterSystem.hpp"
#include "ECS/Test/CounterComponent.hpp"
#include "ECS/Test/TestExclusiveComponent.hpp"

USING_RUKEN_NAMESPACE

int main()
{
    EntityAdmin admin;

    admin.CreateSystem<CounterSystem>();
    admin.CreateExclusiveComponent<TestExclusiveComponent>();

    BENCHMARK("Entity creation")
    {
        for (RkSize index = 0; index < 4092; ++index)
            admin.CreateEntity<CounterComponent, TestTagComponent>();
    }

    Entity entity = admin.CreateEntity<TestTagComponent, CounterComponent>();

    Entity const entity2 = admin.CreateEntity<TestTagComponent, CounterComponent>();
    Entity const entity3 = admin.CreateEntity<TestTagComponent, CounterComponent>();
    Entity const entity4 = admin.CreateEntity<TestTagComponent, CounterComponent>();

    entity4.GetOwner().DeleteEntity(entity4);
    entity3.GetOwner().DeleteEntity(entity3);
    entity .GetOwner().DeleteEntity(entity);

    entity = admin.CreateEntity<TestTagComponent, CounterComponent>();

    admin.StartSimulation();
    admin.UpdateSimulation();
    admin.EndSimulation();

    //Kernel kernel;
    //return kernel.Run();
}
