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

#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"
#include "ECS/ExclusiveComponentBase.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief An exclusive component is a special kind of component;
 *        This component only exists once in the entire entity admin and isn't linked to any entity.
 *        This is useful for storing scene-wide properties like inputs, game states etc...
 *
 * Exclusive components cannot be queried like regular components.
 * Instead, you need to explicitly fetch them via the entity admin.
 *
 * \note Note that views are currently not supported using these components.
 * \tparam TUniqueId Unique ID of the component. This ID does not need to be created using a common pool with the regular components.
 *                   Ideally this would be generated automatically at compile time but doing so in c++ is
 *                   really hard and not 100% reliable. Since this ID must be unique for each component, the best way to maintain it
 *                   is using an enum enumerating every component of the game. This way if a component is deleted, the ids of every other
 *                   component will be maintained automatically. This enum must use the default values in order to work. See examples for more info.
 */
template <RkSize TUniqueId>
class ExclusiveComponent : public ExclusiveComponentBase
{
    public:

        #pragma region Members

        static constexpr RkSize id = TUniqueId;

        #pragma endregion 

        #pragma region Constructors

        ExclusiveComponent()                                  = default;
        ExclusiveComponent(ExclusiveComponent const& in_copy) = default;
        ExclusiveComponent(ExclusiveComponent&&      in_move) = default;
        virtual ~ExclusiveComponent()                         = default;

        #pragma endregion

        #pragma region Operators

        ExclusiveComponent& operator=(ExclusiveComponent const& in_copy) = default;
        ExclusiveComponent& operator=(ExclusiveComponent&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE