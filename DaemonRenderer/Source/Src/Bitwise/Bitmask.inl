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

#pragma region Constructors

template <typename TEnumType>
template <typename... TData, internal::AreEnumType<TEnumType, TData...>>
constexpr Bitmask<TEnumType>::Bitmask(TData... in_data) noexcept:
    Parent(static_cast<UnderlyingType>(std::forward<TData>(in_data))...)
{}

#pragma endregion

#pragma region Methods

template <typename TEnumType>
template <typename... TData, internal::AreEnumType<TEnumType, TData...>>
constexpr DAEbool Bitmask<TEnumType>::HasAll(TData... in_data) const noexcept
{
    return Parent::HasAll(std::forward(static_cast<UnderlyingType>(in_data))...);
}

template <typename TEnumType>
constexpr DAEbool Bitmask<TEnumType>::HasAll(Bitmask<TEnumType> const& in_bitmask) const noexcept
{
    return Parent::HasAll(static_cast<Parent>(*this));
}

template <typename TEnumType>
template <typename... TData, internal::AreEnumType<TEnumType, TData...>>
constexpr DAEbool Bitmask<TEnumType>::HasOne(TData... in_data) const noexcept
{
    return Parent::HasOne(std::forward(static_cast<UnderlyingType>(in_data))...);
}

template <typename TEnumType>
constexpr DAEbool Bitmask<TEnumType>::HasOne(Bitmask<TEnumType> const& in_bitmask) const noexcept
{
    return Parent::HasOne(static_cast<Parent>(*this));
}

template <typename TEnumType>
template <typename... TData, internal::AreEnumType<TEnumType, TData...>>
constexpr DAEvoid Bitmask<TEnumType>::Add(TData... in_data) noexcept
{
    Parent::Add(std::forward(static_cast<UnderlyingType>(in_data))...);
}

template <typename TEnumType>
constexpr DAEvoid Bitmask<TEnumType>::Add(Bitmask<TEnumType> const& in_bitmask) noexcept
{
    Parent::Add(static_cast<Parent>(in_bitmask));
}

template <typename TEnumType>
template <typename... TData, internal::AreEnumType<TEnumType, TData...>>
constexpr DAEvoid Bitmask<TEnumType>::Remove(TData... in_data) noexcept
{
    Parent::Remove(std::forward(static_cast<UnderlyingType>(in_data))...);
}

template <typename TEnumType>
constexpr DAEvoid Bitmask<TEnumType>::Remove(Bitmask<TEnumType> const& in_bitmask) noexcept
{
    Parent::Remove(static_cast<Parent>(in_bitmask));
}

template <typename TEnumType>
template <typename TLambdaType>
constexpr DAEvoid Bitmask<TEnumType>::Foreach(TLambdaType in_lambda) const noexcept
{
    Parent::template Foreach<TLambdaType, UnderlyingType>(std::forward(in_lambda));
}

#pragma endregion

#pragma region Operators

template<typename TEnumType>
constexpr Bitmask<TEnumType> Bitmask<TEnumType>::operator+ (TEnumType const& in_bit) const noexcept
{
    return Parent::operator+(std::forward(static_cast<UnderlyingType>(in_bit)));
}

template<typename TEnumType>
constexpr Bitmask<TEnumType> Bitmask<TEnumType>::operator- (TEnumType const& in_bit) const noexcept
{
    return Parent::operator-(std::forward(static_cast<UnderlyingType>(in_bit)));
}

template<typename TEnumType>
constexpr Bitmask<TEnumType>& Bitmask<TEnumType>::operator-=(TEnumType const& in_bit) noexcept
{
    return Parent::operator-=(std::forward(static_cast<UnderlyingType>(in_bit)));
}

template <typename TEnumType>
constexpr Bitmask<TEnumType>& Bitmask<TEnumType>::operator+=(TEnumType const& in_bit) noexcept
{
    return Parent::operator+=(std::forward(static_cast<UnderlyingType>(in_bit)));
}

#pragma endregion