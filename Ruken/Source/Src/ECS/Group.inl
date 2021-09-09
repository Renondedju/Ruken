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

template <AnyComponentType... TComponents>
Group<TComponents...>::Group(Archetype& in_archetype, TComponents&... in_components) noexcept
    : m_archetype  {in_archetype},
      m_components {std::forward_as_tuple(in_components...)}
{}

template <AnyComponentType... TComponents>
template <ComponentFieldType... TFields>
ComponentView<TFields...> Group<TComponents...>::GetViewInternal(Tag<ComponentView<TFields...>>) noexcept
{
    return ComponentView<TFields...> (m_archetype, std::get<typename TFields::Component&>(m_components).template GetFieldContainer<TFields>().GetHead()...);
}

template <AnyComponentType... TComponents>
template <ViewType TView>
TView Group<TComponents...>::GetView() noexcept
{
    return GetViewInternal(Tag<TView>{}); // This trick is used to extract the fields for the requested view
}

template <AnyComponentType... TComponents>
Archetype& Group<TComponents...>::GetReferencedArchetype() const noexcept
{
    return m_archetype;
}