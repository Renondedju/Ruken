
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
constexpr RkBool Bitmask<TEnumType>::HasAll(TData... in_data) const noexcept
{
    return Parent::HasAll(static_cast<UnderlyingType>(in_data)...);
}

template <typename TEnumType>
constexpr RkBool Bitmask<TEnumType>::HasAll(Bitmask const& in_bitmask) const noexcept
{
    return Parent::HasAll(static_cast<Parent>(*this));
}

template <typename TEnumType>
template <typename... TData, internal::AreEnumType<TEnumType, TData...>>
constexpr RkBool Bitmask<TEnumType>::HasOne(TData... in_data) const noexcept
{
    return Parent::HasOne(static_cast<UnderlyingType>(in_data)...);
}

template <typename TEnumType>
constexpr RkBool Bitmask<TEnumType>::HasOne(Bitmask const& in_bitmask) const noexcept
{
    return Parent::HasOne(static_cast<Parent>(*this));
}

template <typename TEnumType>
template <typename... TData, internal::AreEnumType<TEnumType, TData...>>
constexpr RkVoid Bitmask<TEnumType>::Add(TData... in_data) noexcept
{
    Parent::Set(static_cast<UnderlyingType>(in_data)...);
}

template <typename TEnumType>
constexpr RkVoid Bitmask<TEnumType>::Add(Bitmask const& in_bitmask) noexcept
{
    Parent::Set(static_cast<Parent>(in_bitmask));
}

template <typename TEnumType>
template <typename... TData, internal::AreEnumType<TEnumType, TData...>>
constexpr RkVoid Bitmask<TEnumType>::Remove(TData... in_data) noexcept
{
    Parent::Clear(static_cast<UnderlyingType>(in_data)...);
}

template <typename TEnumType>
constexpr RkVoid Bitmask<TEnumType>::Remove(Bitmask const& in_bitmask) noexcept
{
    Parent::Clear(static_cast<Parent>(in_bitmask));
}

template <typename TEnumType>
template <typename TLambdaType>
constexpr RkVoid Bitmask<TEnumType>::Foreach(TLambdaType in_lambda) const noexcept
{
    Parent::template Foreach<TLambdaType, TEnumType>(std::forward<TLambdaType>(in_lambda));
}

#pragma endregion

#pragma region Operators

template<typename TEnumType>
constexpr Bitmask<TEnumType> Bitmask<TEnumType>::operator+ (TEnumType const& in_bit) const noexcept
{
    return Parent::operator+(static_cast<UnderlyingType>(in_bit));
}

template<typename TEnumType>
constexpr Bitmask<TEnumType> Bitmask<TEnumType>::operator- (TEnumType const& in_bit) const noexcept
{
    return Parent::operator-(static_cast<UnderlyingType>(in_bit));
}

template<typename TEnumType>
constexpr Bitmask<TEnumType>& Bitmask<TEnumType>::operator-=(TEnumType const& in_bit) noexcept
{
    return Parent::operator-=(static_cast<UnderlyingType>(in_bit));
}

template <typename TEnumType>
constexpr Bitmask<TEnumType>& Bitmask<TEnumType>::operator+=(TEnumType const& in_bit) noexcept
{
    return Parent::operator+=(static_cast<UnderlyingType>(in_bit));
}

#pragma endregion