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

template <ComponentFieldType... TFields>
template <RkSize... TIds>
RkSize Component<TFields...>::EnsureStorageSpaceHelper(RkSize in_size, std::index_sequence<TIds...>) noexcept
{
        return MinExceptZero<RkSize>(
        { // Initializer list containing all the lambda invocations
            [in_size](FieldContainerType<TFields>& in_list)
            {
                // Computes the chunk id required to store all the requested data
                RkSize const chunk_id      { in_size / in_list.chunk_element_count };
                RkSize const chunk_missing { (chunk_id + 1) - in_list.GetSize() };

                // If the size isn't big enough, allocating some more nodes to fit everything
                for (RkSize index = 0; index < chunk_missing; ++index)
                    in_list.CreateNode();

                // Returning the amount of elements created
                return chunk_missing * in_list.chunk_element_count;

            }(std::get<TIds>(m_storage))...
        }
    );
}

template <ComponentFieldType... TFields>
Component<TFields...>::Component(Archetype const* in_owning_archetype) noexcept:
    ComponentBase {in_owning_archetype}
{ }  

template <ComponentFieldType... TFields>
RkSize Component<TFields...>::EnsureStorageSpace(RkSize const in_size) noexcept
{
    return EnsureStorageSpaceHelper(in_size, std::make_index_sequence<sizeof...(TFields)>());
}

template <ComponentFieldType... TFields>
template <ComponentFieldType TField>
typename Component<TFields...>::template FieldContainerType<TField>& Component<TFields...>::GetFieldContainer() noexcept
{
    return std::get<Helper::template FieldIndex<TField>::value>(m_storage);
}
