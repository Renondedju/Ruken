
#pragma once

#include "Build/Namespace.hpp"
#include "Types/Concepts/FundamentalType.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Eases the creation of strong types
 * 
 * Every method is constexpr and noexcept if the base type is noexcept as well
 * 
 * To create a "complex" StrongType, simply inherit from this class and overload all the required operators.
 * Do note that every operator class like Addition or Modulo, can also be inherited to save you some time.
 * 
 * Example :
 * 
 * // This class is strongly typed and only has the addition and subtraction operators available
 * struct Meter : StrongType<float, Meter>, Addition<Meter>, Subtraction<Meter>
 * { using StrongType::StrongType };
 * 
 * \tparam TBase Base type, could be any fundamental type
 * \tparam TUniquePhantom Phantom type, this should be unique for each type
 * \see https://foonathan.net/blog/2016/10/19/strong-typedefs.html
 * \see https://arne-mertz.de/2016/11/stronger-types/
 *
 * \warning Please use the RUKEN_EMPTY_BASES macro with MSVC in order to avoid EBO miss
 */
template <FundamentalType TBase, typename TUniquePhantom>
class StrongType
{
    #pragma region Members

    TBase m_value {};

    #pragma endregion

    public:

        using UnderlyingType = TBase;

        #pragma region Constructors

        template<typename TType>
        requires std::is_convertible_v<TType, TBase>
        explicit constexpr StrongType(TType const& in_copy) noexcept:
            m_value {static_cast<TBase>(in_copy)}
        {}

        template<typename TType>
        requires std::is_convertible_v<TType, TBase>
        explicit constexpr StrongType(TType&& in_move) noexcept:
            m_value {static_cast<TBase>(std::forward<TType>(in_move))}
        {}

        constexpr StrongType()                          = default;
        constexpr StrongType(StrongType const& in_copy) = default;
        constexpr StrongType(StrongType&&      in_move) = default;
                 ~StrongType()                          = default;

        #pragma endregion

        #pragma region Operators

        template <typename TType>
        requires std::is_convertible_v<TBase, TType>
        explicit constexpr operator TType&() noexcept
        {
            return m_value;
        }

        template <typename TType>
        requires std::is_convertible_v<TBase, TType>
        explicit constexpr operator TType const&() const noexcept
        {
            return m_value;
        }

        constexpr StrongType& operator=(StrongType const& in_copy) = default;
        constexpr StrongType& operator=(StrongType&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE