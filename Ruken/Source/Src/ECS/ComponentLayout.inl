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

#pragma region Methods

template <typename ... TFields>
template <typename TLayoutView, RkSize... TIds>
constexpr auto ComponentLayout<TFields...>::GetHelper(ContainerType& in_container, RkSize in_position, std::index_sequence<TIds...>) noexcept
{
    // Guaranteed copy elision
    return TLayoutView { std::reference_wrapper(std::get<TIds>(in_container)[in_position])... };
}

template <typename ... TFields>
template <RkSize... TIds>
constexpr RkVoid ComponentLayout<TFields...>::InsertHelper(ContainerType& in_container, RkSize in_index, Item&& in_item, std::index_sequence<TIds...>) noexcept
{
    (std::get<TIds>(in_container).InsertData(in_index, std::forward(std::get<TIds>(in_item))), ...);
}

template <typename ... TFields>
template <typename TLayoutView>
constexpr auto ComponentLayout<TFields...>::Get(ContainerType& in_container, RkSize in_position) noexcept
{
    return GetHelper<TLayoutView>(in_container, in_position, typename TLayoutView::Sequence());
}

template <typename ... TFields>
constexpr RkVoid ComponentLayout<TFields...>::Insert(ContainerType& in_container, RkSize in_index, Item&& in_item) noexcept
{
    InsertHelper(in_container, in_index, std::forward<Item>(in_index), std::make_index_sequence<sizeof...(TFields)>());
}

#pragma endregion
