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

template <template <typename> class TContainer, typename ... TLayoutTypes>
template <typename TLayoutView, DAEsize... TIds>
constexpr auto DataLayout<TContainer, TLayoutTypes...>::GetHelper(
    ContainerType& in_container, DAEsize in_position, std::index_sequence<TIds...>) noexcept
{
    // Guaranteed copy elision
    return TLayoutView { std::reference_wrapper(std::get<TIds>(in_container)[in_position])... };
}

template <template <typename> class TContainer, typename ... TLayoutTypes>
template <DAEsize... TIds>
constexpr DAEvoid DataLayout<TContainer, TLayoutTypes...>::ResizeHelper(
    ContainerType& in_container, DAEsize in_size, std::index_sequence<TIds...>) noexcept
{
    (std::get<TIds>(in_container).resize(in_size), ...);
}

template <template <typename> class TContainer, typename ... TLayoutTypes>
template <typename TValue, DAEsize... TIds>
constexpr DAEvoid DataLayout<TContainer, TLayoutTypes...>::PushBackHelper(
    ContainerType& in_container, TValue&& in_value, std::index_sequence<TIds...>) noexcept
{
    // Fold expression
    (std::get<TIds>(in_container).push_back(std::get<TIds>(static_cast<std::tuple<TLayoutTypes...>&&>(std::forward<TValue>(in_value)))), ...);
}

template <template <typename> class TContainer, typename ... TLayoutTypes>
template <typename TLayoutView>
constexpr auto DataLayout<TContainer, TLayoutTypes...>::Get(
	ContainerType& in_container, DAEsize in_position) noexcept
{
    return GetHelper<TLayoutView>(in_container, in_position, typename TLayoutView::Sequence());
}

template <template <typename> class TContainer, typename ... TLayoutTypes>
constexpr DAEvoid DataLayout<TContainer, TLayoutTypes...>::Resize(
	ContainerType& in_container, DAEsize in_size) noexcept
{
    ResizeHelper(in_container, in_size, std::make_index_sequence<sizeof...(TLayoutTypes)>());
}

template <template <typename> class TContainer, typename ... TLayoutTypes>
template <typename TValue>
constexpr DAEvoid DataLayout<TContainer, TLayoutTypes...>::PushBack(
	ContainerType& in_container, TValue&& in_value) noexcept
{
    PushBackHelper(in_container, std::forward<TValue>(in_value), std::make_index_sequence<sizeof...(TLayoutTypes)>());
}

template <template <typename> class TContainer, typename ... TLayoutTypes>
constexpr DAEsize DataLayout<TContainer, TLayoutTypes...>::Size(
    ContainerType const& in_container) noexcept
{
    return std::get<0>(in_container).size();
}
