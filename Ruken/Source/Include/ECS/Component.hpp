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

#include "Config.hpp"

#include "Meta/Assert.hpp"
#include "Containers/SOA/DataLayout.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief This class is the actual component class used to store the items.
 * \tparam TItem Associated item of the component, must be a subtype of ComponentItem
 * \tparam TUniqueId Unique ID of the component.
 *                   Ideally this would be generated automatically at compile time but doing so in c++ is
 *                   really hard and not 100% reliable. Since this ID must be unique for each component, the best way to maintain it
 *                   is using an enum enumerating every component of the game. This way if a component is deleted, the ids of every other
 *                   component will be maintained automatically. This enum must use the default values in order to work. See examples for more info.
 */
template <typename TItem, RkSize TUniqueId>
class Component
{
     RUKEN_STATIC_ASSERT(TUniqueId < RUKEN_MAX_ECS_COMPONENTS, "Please increate the maximum amount of ECS components to run this program.");

    private:

        #pragma region Members

        // Storage of the component
        typename TItem::Layout::ContainerType m_storage;

        #pragma endregion 

    public:

        using Layout = typename TItem::Layout;
        using Item   = TItem;
        using ItemId = RkSize;

        static constexpr RkSize id = TUniqueId;

        #pragma region Constructors

        Component()                         = default;
        Component(Component const& in_copy) = default;
        Component(Component&&      in_move) = default;
        ~Component()                        = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Creates an item into the component
         * \param in_item item to push back
         * \return Created item id
         */
        ItemId CreateItem(TItem&& in_item);
        ItemId CreateItem();

        /**
         * \brief Returns the count of items in this component
         * \return Component item count
         */
        RkSize GetItemCount() const noexcept;

        #pragma endregion 

        #pragma region Operators

        Component& operator=(Component const& in_copy) = default;
        Component& operator=(Component&&      in_move) = default;

        #pragma endregion
};

/**
 * \brief Shorthand to declare a component alias named "<in_component_name>Component"
 * \note The component item must be named "<in_component_name>ComponentItem"
 * \note This macro also enables the usage of an enum class for the component table enum
 * \param in_component_table Component table enum
 * \param in_component_name Name of the component as described in the above component table enum
 */
#define RUKEN_DEFINE_COMPONENT(in_component_table, in_component_name)\
    using in_component_name##Component = Component<in_component_name##ComponentItem, static_cast<RkSize>(in_component_table::in_component_name)>

#include "ECS/Component.inl"

END_RUKEN_NAMESPACE