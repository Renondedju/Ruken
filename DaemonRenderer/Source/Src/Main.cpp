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

#include <iostream>
#include <Vector/Vector.hpp>

#include "Config.hpp"

#include "ECS/Component.hpp"
#include "ECS/EntityAdmin.hpp"
#include "ECS/ComponentItem.hpp"
#include "ECS/ComponentSystem.hpp"

#include "Debug/Logging/Logger.hpp"

USING_DAEMON_NAMESPACE

/**
 * \brief This table keeps track of every available component in the ECS
 *        it is also used to create and maintain every component ID,
 *        see the Component class for more info.
 */
enum class EComponentsTable
{
    Position,
    Counter,
    Life,
};

struct PositionComponentItem : public ComponentItem<Vector3f>
{
    enum class EMembers
    { Position };
};

struct LifeComponentItem : public ComponentItem<DAEfloat, DAEfloat>
{
    enum class EMembers
    { Life, MaxLife };
};

struct CounterComponentItem : public ComponentItem<DAEfloat> 
{
    enum class EMembers
    { Counter };
};

DAEMON_DEFINE_COMPONENT(EComponentsTable, Position);
DAEMON_DEFINE_COMPONENT(EComponentsTable, Counter);
DAEMON_DEFINE_COMPONENT(EComponentsTable, Life);

int main()
{
    EntityAdmin admin;

    admin.CreateEntity<LifeComponent>();
    admin.CreateEntity<LifeComponent>();
    admin.CreateEntity<LifeComponent, CounterComponent>();
    admin.CreateEntity<LifeComponent, PositionComponent>();
    admin.CreateEntity<PositionComponent, LifeComponent>();
    admin.CreateEntity<PositionComponent, LifeComponent, CounterComponent>();
    admin.CreateEntity<PositionComponent, LifeComponent, CounterComponent>();

    ComponentQuery query;
    query.SetupInclusionQuery<LifeComponent, PositionComponent>();
    query.SetupExclusionQuery<CounterComponent>();

    std::cout << query.Match(Archetype<LifeComponent>())                                      << std::endl;
    std::cout << query.Match(Archetype<LifeComponent, CounterComponent>())                    << std::endl;
    std::cout << query.Match(Archetype<LifeComponent, PositionComponent>())                   << std::endl;
    std::cout << query.Match(Archetype<PositionComponent, LifeComponent>())                   << std::endl;
    std::cout << query.Match(Archetype<PositionComponent, LifeComponent, CounterComponent>()) << std::endl;

    system("pause");

    return EXIT_SUCCESS;
}