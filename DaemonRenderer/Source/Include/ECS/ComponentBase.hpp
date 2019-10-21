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

#include <type_traits>

#include "Config.hpp"
#include "ECS/ComponentTypeIdIterator.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * Component base class.
 * This class is the actual component class used to store the items.
 * This class has to be instantiated in order to be used by a system.
 */
template <typename TItem>
class __declspec(novtable) ComponentBase : public ComponentTypeIdIterator
{
    private:

        #pragma region Members

        // Storage of the component
        typename TItem::Layout::ContainerType m_storage;

        #pragma endregion 

    public:

        using Layout = typename TItem::Layout;
        using Item   = TItem;
        using ItemId = DAEsize;

        #pragma region Constructors

        ComponentBase()                             noexcept = default;
        ComponentBase(ComponentBase const& in_copy) noexcept = default;
        ComponentBase(ComponentBase&&      in_move) noexcept = default;
        ~ComponentBase()                            noexcept = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the statically evaluated unique type id of the component
         * \return Type id
         */
        static DAEsize TypeId() noexcept;

        /**
         * \brief Creates an item into the component
         * \param in_item item to push back
         * \return Created item id
         */
        ItemId CreateItem(TItem&& in_item);

        #pragma endregion 

        #pragma region Operators

        ComponentBase& operator=(ComponentBase const& in_copy) noexcept = default;
        ComponentBase& operator=(ComponentBase&&      in_move) noexcept = default;

        #pragma endregion
};

#include "ECS/ComponentBase.inl"

END_DAEMON_NAMESPACE