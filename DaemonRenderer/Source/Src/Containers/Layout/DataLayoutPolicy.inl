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

template <template <typename...> class TContainer, typename TItem>
constexpr typename DataLayoutPolicy<TContainer, EDataLayout::ArrayOfStructures, TItem>::ValueType
    DataLayoutPolicy<TContainer, EDataLayout::ArrayOfStructures, TItem>::Get(
		ContainerType& in_container,
		DAEsize in_position)
{
    return ValueType(*static_cast<typename TItem::Representation*>(&in_container[in_position]));
}

template <template <typename...> class TContainer, typename TItem>
constexpr DAEvoid DataLayoutPolicy<TContainer, EDataLayout::ArrayOfStructures, TItem>::Resize(
    ContainerType& in_container,
    DAEsize        in_size)
{
    in_container.resize(in_size);
}

template <template <typename...> class TContainer, typename TItem>
template <typename TValue>
constexpr DAEvoid DataLayoutPolicy<TContainer, EDataLayout::ArrayOfStructures, TItem>::PushBack(
    ContainerType& in_container,
    TValue&&       in_value)
{
    in_container.push_back(in_value);
}

template <template <typename...> class TContainer, typename TItem>
constexpr DAEsize DataLayoutPolicy<TContainer, EDataLayout::ArrayOfStructures, TItem>::Size(
    ContainerType& in_container)
{
    return in_container.size();
}

template <template <typename...> class TContainer, typename TItem>
constexpr typename DataLayoutPolicy<TContainer, EDataLayout::StructureOfArrays, TItem>::ValueType
    DataLayoutPolicy<TContainer, EDataLayout::StructureOfArrays, TItem>::Get(
		ContainerType& in_container,
		DAEsize in_position)
{
    // Unrolling parameter pack
    return GetHelper(in_container, in_position, std::make_integer_sequence<DAEsize, sizeof...(typename TItem::RepresentationLayout)>());
}

template <template <typename...> class TContainer, typename TItem>
constexpr DAEvoid DataLayoutPolicy<TContainer, EDataLayout::StructureOfArrays, TItem>::Resize(
    ContainerType& in_container,
    DAEsize        in_size)
{
    // Unrolling parameter pack
    ResizeHelper(in_container, in_size, std::make_integer_sequence<DAEsize, sizeof...(typename TItem::RepresentationLayout)>());
}

template <template <typename...> class TContainer, typename TItem>
template <typename TValue>
constexpr DAEvoid DataLayoutPolicy<TContainer, EDataLayout::StructureOfArrays, TItem>::PushBack(
    ContainerType& in_container,
    TValue&&       in_value)
{
    // Unrolling parameter pack
    PushBackHelper(in_container, std::forward<TValue>(in_value), std::make_integer_sequence<DAEsize, sizeof...(typename TItem::RepresentationLayout)>());
}

template <template <typename...> class TContainer, typename TItem>
constexpr DAEsize DataLayoutPolicy<TContainer, EDataLayout::StructureOfArrays, TItem>::Size(
    ContainerType& in_container)
{
    return std::get<0>(in_container).size();
}

template <template <typename...> class TContainer, typename TItem>
template <unsigned... TIds>
constexpr auto DataLayoutPolicy<TContainer, EDataLayout::StructureOfArrays, TItem>::GetHelper(
    ContainerType&                         in_container,
    DAEsize                                in_position,
    std::integer_sequence<DAEsize, TIds...>)
{
    // Guaranteed copy elision
    return ValueType{ReferenceWrapper(std::get<TIds>(in_container)[in_position])...};
}

template <template <typename...> class TContainer, typename TItem>
template <unsigned... TIds>
constexpr DAEvoid DataLayoutPolicy<TContainer, EDataLayout::StructureOfArrays, TItem>::ResizeHelper(
    ContainerType&                         in_container,
    DAEsize                                in_size,
    std::integer_sequence<DAEsize, TIds...>)
{
    // Fold expression
    (std::get<TIds>(in_container).resize(in_size), ...);
}

template <template <typename...> class TContainer, typename TItem>
template <typename TValue, DAEsize... TIds>
constexpr DAEvoid DataLayoutPolicy<TContainer, EDataLayout::StructureOfArrays, TItem>::PushBackHelper(
    ContainerType&                         in_container,
    TValue&&                               in_value,
    std::integer_sequence<DAEsize, TIds...>)
{
    // Fold expression
    (std::get<TIds>(in_container).push_back(std::get<TIds>(std::forward<TValue>(in_value))), ...);
}
