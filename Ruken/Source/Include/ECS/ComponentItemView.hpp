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

#include <type_traits>

#include "Build/Namespace.hpp"

#include "Meta/CopyConst.hpp"
#include "Meta/TupleIndex.hpp"
#include "Meta/TupleHasType.hpp"
#include "Containers/SOA/DataLayoutView.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Allows to fetch only the required fields in a component, saving on data bus bandwidth and cache
 * \note All instances of this class are generated via the item type of each component
 * \tparam TPack Index pack enumerating the index of the members to fetch
 * \tparam TMembers Member types to create a reference onto
 */
template <typename TPack, typename... TMembers>
struct ComponentItemView;

template <template <DAEsize...> class TPack, DAEsize... TIndices, typename... TMembers>
struct ComponentItemView<TPack<TIndices...>, TMembers...> : public DataLayoutView<std::index_sequence<TIndices...>, CopyConst<TMembers, typename TMembers::Type>...>
{
    private:

        /**
         * \brief Returns the index of a member inside of the view
         * \tparam TMember Member to look for
         */
        template <typename TMember>
        using MemberIndex = TupleIndex<std::remove_const_t<TMember>, std::tuple<std::remove_const_t<TMembers>...>>;

        /**
         * \brief Checks if a given member exists in this view,
         *        if not SFINAE will prevent compilation to avoid some more ugly errors 
         * \tparam TMember Member to look for
         */
        template <typename TMember>
        using MemberExists = std::enable_if_t<TupleHasType<TMember, std::tuple<std::remove_const_t<TMembers>...>>::value, DAEbool>;

    public:

        // Making parent constructors available
        using DataLayoutView<std::index_sequence<TIndices...>, CopyConst<TMembers, typename TMembers::Type>...>::DataLayoutView;
        using DataLayoutView<std::index_sequence<TIndices...>, CopyConst<TMembers, typename TMembers::Type>...>::operator=;

        /**
         * \brief Returns a reference onto a given member stored in the view.
         * \note The view must include the requested member to work
         * \tparam TMember Requested member. You only must pass the Member representative type without any cv ref/ptr attributes.
         * \return Member reference
         */
        template<typename TMember, MemberExists<TMember> = true>
        auto&       Fetch()       { return std::get<MemberIndex<TMember>::value>(*this); }

        /**
         * \brief Returns a constant reference onto a given member stored in the view.
         * \note The view must include the requested member to work
         * \tparam TMember Requested member. You only must pass the Member representative type without any cv ref/ptr attributes.
         * \return Member constant reference
         */
        template<typename TMember, MemberExists<TMember> = true>
        auto const& Fetch() const { return std::get<MemberIndex<TMember>::value>(*this); }
};

END_DAEMON_NAMESPACE
