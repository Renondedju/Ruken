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

template <EEventName TEventName, ComponentType ... TComponents>
EventHandler<TEventName, TComponents...>::EventHandler() noexcept
{
    // This lambda just helps unwrapping the IterativeComponents tuple into the SetupInclusionQuery function call
    [&]<RkSize... TIds>(std::index_sequence<TIds...>){
        m_query.SetupInclusionQuery<std::tuple_element_t<TIds, IterativeComponents>...>();
    }(std::make_index_sequence<std::tuple_size_v<IterativeComponents>>());
}

template <EEventName TEventName, ComponentType ... TComponents>
ComponentQuery const& EventHandler<TEventName, TComponents...>::GetQuery() const noexcept
{
    return m_query;
}

template <EEventName TEventName, ComponentType ... TComponents>
RkVoid EventHandler<TEventName, TComponents...>::AddReferenceGroup(Archetype& in_archetype) noexcept
{
    // This lambda just helps unwrapping the IterativeComponents tuple into the CreateGroupReference function call
    [&]<RkSize... TIds>(std::index_sequence<TIds...>){
        m_groups.emplace_back(in_archetype.CreateGroupReference<std::tuple_element_t<TIds, IterativeComponents>...>());
    }(std::make_index_sequence<std::tuple_size_v<IterativeComponents>>());
}

template <EEventName TEventName, ComponentType ... TComponents>
EEventName EventHandler<TEventName, TComponents...>::GetHandledEvent() noexcept
{
    return TEventName;
}
