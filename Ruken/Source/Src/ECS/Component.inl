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

template <DAEsize TUniqueId, typename TItem>
EntityID Component<TUniqueId, TItem>::CreateItem(TItem&& in_item) noexcept
{
    Layout::PushBack(m_storage, std::forward<TItem>(in_item));

    return EntityID(Layout::Size(m_storage) - 1);
}

template <DAEsize TUniqueId, typename TItem>
EntityID Component<TUniqueId, TItem>::CreateItem() noexcept
{
    Layout::PushBack(m_storage, std::forward<TItem>(TItem{}));

    return EntityID(Layout::Size(m_storage) - 1);
}

template <DAEsize TUniqueId, typename TItem>
DAEsize Component<TUniqueId, TItem>::GetItemCount() const noexcept
{
    return Layout::Size(m_storage);
}

template <DAEsize TUniqueId, typename TItem>
template <typename TView>
auto Component<TUniqueId, TItem>::GetItemView(EntityID in_entity) noexcept
{
    return Layout::Get<TView>(m_storage, static_cast<DAEsize>(in_entity));
}