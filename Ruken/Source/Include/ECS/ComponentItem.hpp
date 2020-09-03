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

#include <tuple>
#include <vector>

#include "Build/Namespace.hpp"

#include "Meta/IndexPack.hpp"
#include "Meta/TupleIndex.hpp"
#include "ECS/ComponentItemView.hpp"
#include "Containers/SOA/LinkedChunkLayoutItem.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Describes the memory layout of a component to the ECS
 * \tparam TField Variable types
 */
template <typename... TField>
class ComponentItem : public LinkedChunkLayoutItem<typename TField::Type...>
{
    private:

        /**
         * \brief Returns the index of a member using the member class
         * \tparam TMember Member class
         */
        template <typename TMember>
        using VariableIndex = TupleIndex<std::remove_const_t<TMember>, std::tuple<TField...>>;

    public:

        // Default constructor
        ComponentItem(typename TField::Type&&... in_data) noexcept:
            LinkedChunkLayoutItem<typename TField::Type...>(std::forward<typename TField::Type>(in_data)...)
        {}

        // Exposing parent constructors
        using LinkedChunkLayoutItem<typename TField::Type...>::LinkedChunkLayoutItem;
        using LinkedChunkLayoutItem<typename TField::Type...>::operator=;

        // View constructors
        template <typename... TSelectedVariables>
        using MakeView         = ComponentItemView<IndexPack<VariableIndex<TSelectedVariables>::value...>, TSelectedVariables...>;
        using FullView         = MakeView<TField...>;
        using FullReadonlyView = MakeView<TField const...> const;
};

END_DAEMON_NAMESPACE
