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

template <typename... TFields>
template <typename TLayoutView, RkSize... TIds>
TLayoutView ComponentLayout<TFields...>::GetViewHelper(ContainerType& in_container, std::index_sequence<TIds...>) noexcept
{
    // Guaranteed copy elision
    return TLayoutView { std::get<TIds>(in_container).GetHead()... };
}

template <typename... TFields>
template <RkSize... TIds>
RkVoid ComponentLayout<TFields...>::EnsureStorageSpaceHelper(ContainerType& in_container, RkSize in_size, std::index_sequence<TIds...>) noexcept
{
    // Fold expression applying the bellow lambda for each linked chunk list

    ([in_size](LinkedChunkList<typename TFields::Type>& in_list){
        // Computes the chunk id required to store all the requested data
        RkSize const chunk_id = in_size / in_list.chunk_element_count;

        // If the size isn't big enough, allocating some more nodes to fit everything
        while (in_list.GetSize() <= chunk_id)
            in_list.CreateNode();

    }(std::get<TIds>(in_container)), ...);
}

template <typename... TFields>
template <typename TLayoutView>
TLayoutView ComponentLayout<TFields...>::GetView(ContainerType& in_container) noexcept
{
    return GetViewHelper<TLayoutView>(in_container, typename TLayoutView::Sequence());
}

template <typename... TFields>
RkVoid ComponentLayout<TFields...>::EnsureStorageSpace(ContainerType& in_container, RkSize in_size) noexcept
{
    EnsureStorageSpaceHelper(in_container, in_size, std::make_index_sequence<sizeof...(TFields)>());
}

#pragma endregion
