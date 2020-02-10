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

#include "ECS/Component.hpp"
#include "ECS/ComponentItem.hpp"
#include "ECS/ComponentSystem.hpp"

USING_DAEMON_NAMESPACE

struct LifeComponentItem : public ComponentItem<DAEfloat, DAEfloat>
{
    enum EMembers
    {
        Life,
        MaxLife
    };
};

using LifeComponent = Component<LifeComponentItem>;
using LifeView      = LifeComponentItem::FullView;

int main()
{
    
    LifeComponent life_component;
    
    for (int i = 0; i < 200; ++i)
        life_component.CreateItem();

    for (int i = 0; i < 200; ++i)
        LifeComponent::Layout::Get<LifeView>(life_component.m_storage, i).Fetch<LifeComponentItem::MaxLife>() = i;
        
    return EXIT_SUCCESS;
}