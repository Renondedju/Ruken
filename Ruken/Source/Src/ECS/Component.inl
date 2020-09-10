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

template <RkSize TUniqueId, typename... TMembers>
RkVoid Component<TUniqueId, TMembers...>::CreateItemAt(RkSize in_index, Item&& in_item) noexcept
{
    Layout::Insert(m_storage, in_index, std::forward<Item>(in_item));
}

template <RkSize TUniqueId, typename... TMembers>
RkVoid Component<TUniqueId, TMembers...>::CreateItemAt(RkSize in_index) noexcept
{
    CreateItemAt(in_index, Item {});
}

template <RkSize TUniqueId, typename... TMembers>
template <typename TView>
TView Component<TUniqueId, TMembers...>::GetView() noexcept
{
    return Layout::template GetView<TView>(m_storage);
}