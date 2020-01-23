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

#include "Config.hpp"

#include "ECS/Archetype.hpp"
#include "ECS/Component.hpp"
#include "ECS/ComponentItem.hpp"
#include "ECS/ComponentSystem.hpp"
#include "ECS/ArchetypeFingerprint.hpp"

#include "Vector/Vector.hpp"

USING_DAEMON_NAMESPACE

struct LifeComponentItem : public ComponentItem<DAEfloat, DAEfloat>
{
    enum EMembers
    {
        Life,
        MaxLife
    };
    
    auto&       GetLife()       { return std::get<Life>(*this); }
    auto const& GetLife() const { return std::get<Life>(*this); }

    auto&       GetMaxLife()       { return std::get<MaxLife>(*this); }
    auto const& GetMaxLife() const { return std::get<MaxLife>(*this); }
};

struct PositionComponentItem : public ComponentItem<DAEfloat, DAEfloat, DAEfloat>
{
    enum EMembers { X, Y, Z };
    
    auto&       GetX()       { return std::get<X>(*this); }
    auto const& GetX() const { return std::get<X>(*this); }

    auto&       GetY()       { return std::get<Y>(*this); }
    auto const& GetY() const { return std::get<Y>(*this); }

    auto&       GetZ()       { return std::get<Z>(*this); }
    auto const& GetZ() const { return std::get<Z>(*this); }
};

using LifeComponent     = Component<LifeComponentItem>;
using PositionComponent = Component<PositionComponentItem>;

int main()
{
    ArchetypeFingerprint fingerprint;

    fingerprint.AddTrait(LifeComponent::TypeId());
    fingerprint.AddTrait(PositionComponent::TypeId());

    Archetype<LifeComponent, PositionComponent> archetype;
    ComponentSystem<LifeComponent>              life_system;

    for (int i = 0; i < 200; ++i)
        archetype.CreateEntity();

    system("pause");
	
    return EXIT_SUCCESS;
}
