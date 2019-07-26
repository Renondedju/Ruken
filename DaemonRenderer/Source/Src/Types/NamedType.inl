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

template <typename TBase, typename TUniquePhantom>
template <typename ...TArgs, typename>
constexpr NamedType<TBase, TUniquePhantom>::NamedType(TArgs&&... in_args) noexcept(noexcept(TBase(std::forward<TArgs>(in_args)...))):
	m_value {TBase(std::forward<TArgs>(in_args)...)}
{}

template <typename TBase, typename TUniquePhantom>
constexpr NamedType<TBase, TUniquePhantom>::NamedType(TBase const& in_base) noexcept(noexcept(TBase(in_base))):
	m_value {in_base}
{}

template <typename TBase, typename TUniquePhantom>
constexpr NamedType<TBase, TUniquePhantom>::NamedType(NamedType const& in_copy) noexcept(std::is_nothrow_copy_constructible_v<TBase>):
	m_value {in_copy}
{}

template <typename TBase, typename TUniquePhantom>
constexpr NamedType<TBase, TUniquePhantom>::NamedType(NamedType&& in_move) noexcept(std::is_nothrow_move_constructible_v<TBase>):
	m_value {std::forward<TBase>(in_move)}
{}

template <typename TBase, typename TUniquePhantom>
constexpr NamedType<TBase, TUniquePhantom>::NamedType() noexcept(std::is_nothrow_constructible_v<TBase>):
	m_value {}
{}

template <typename TBase, typename TUniquePhantom>
constexpr NamedType<TBase, TUniquePhantom>::operator TBase&() noexcept
{
	return m_value;
}

template <typename TBase, typename TUniquePhantom>
constexpr NamedType<TBase, TUniquePhantom>::operator TBase const& () const noexcept
{
	return m_value;	
}

template <typename TBase, typename TUniquePhantom>
constexpr NamedType<TBase, TUniquePhantom>& NamedType<TBase, TUniquePhantom>::
	operator=(NamedType const& in_copy) noexcept(std::is_nothrow_copy_assignable_v<TBase>)
{
	m_value = std::forward<TBase>(in_copy.m_value);
	return *this;
}

template <typename TBase, typename TUniquePhantom>
constexpr NamedType<TBase, TUniquePhantom>& NamedType<TBase, TUniquePhantom>::
	operator=(NamedType&& in_move) noexcept(std::is_nothrow_move_assignable_v<TBase>)
{
	m_value = std::forward<TBase>(in_move.m_value);
	return *this;
}