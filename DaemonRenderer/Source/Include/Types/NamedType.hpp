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

#pragma once

#include <type_traits>

#include "Config.hpp"

#include "Meta/Meta.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief A named type allows the creation of strong types more easily
 * 
 * Every method is constexpr and noexcept if the base type is noexcept as well
 * 
 * To create a "complex" NamedType, simply inherit from this class and overload all the required operators.
 * Do note that every operator class like Addition or Modulo, can also be inherited to save you some time.
 * 
 * Example :
 * 
 * // This class is strongly typed and only has the addition and subtraction operators available
 * struct Meter : NamedType<float, Meter>, Addition<Meter>, Subtraction<Meter>
 * { using NamedType::NamedType };
 * 
 * \tparam TBase          Base type, could be any fundamental type
 * \tparam TUniquePhantom Phantom type, this should be unique for each type
 * 
 * \see https://foonathan.net/blog/2016/10/19/strong-typedefs.html
 * \see https://arne-mertz.de/2016/11/stronger-types/
 */
template <typename TBase, typename TUniquePhantom>
class NamedType
{
    private:

        #pragma region Variables

        TBase m_value;

        #pragma endregion

    public:

        using UnderlyingType = TBase;

        #pragma region Constructors

        /**
         * \brief Constructs the content of the NamedType using the following constructor: TStrongType(TArgs...)
         *
         * \tparam TArgs Arguments type
         *
         * \param in_args arguments
         */
        template <typename ...TArgs, typename = std::enable_if_t<std::is_constructible_v<TBase, TArgs...>>>
        constexpr NamedType(TArgs&&... in_args) noexcept(noexcept(TBase(std::forward<TArgs>(in_args)...)));

        /**
         * \brief Base init constructor
         *
         * \param in_base base value
         */
        explicit constexpr NamedType(TBase const&       in_base) noexcept(noexcept(TBase(in_base)));

        /**
         * \brief Explicit const expression copy operator.
         *
         * This operator is noexcept if the corresponding base type operator is noexcept itself
         * 
         * \param in_copy Value to copy from
         */
        explicit constexpr NamedType(NamedType const&   in_copy) noexcept(std::is_nothrow_copy_constructible_v<TBase>);
    
        /**
         * \brief Explicit const expression move operator.
         *
         * This operator is noexcept if the corresponding base type operator is noexcept itself
         * 
         * \param in_move Value to move from
         */
        explicit constexpr NamedType(NamedType&&        in_move) noexcept(std::is_nothrow_move_constructible_v<TBase>);

        // Default constructor / destructor
        constexpr NamedType () noexcept(std::is_nothrow_constructible_v<TBase>);
                  ~NamedType() noexcept(std::is_nothrow_destructible_v <TBase>) = default;

        #pragma endregion

        #pragma region Operators

        explicit constexpr operator TBase&       () noexcept;
        explicit constexpr operator TBase const& () const noexcept;

        constexpr NamedType& operator=(NamedType const& in_copy) noexcept(std::is_nothrow_copy_assignable_v<TBase>);
        constexpr NamedType& operator=(NamedType&&      in_move) noexcept(std::is_nothrow_move_assignable_v<TBase>);

        #pragma endregion
};

#include "Types/NamedType.inl"

// --- Underlying Type

namespace internal
{
    /**
     * \brief Implementation of the UnderlyingType method
     *
     * \tparam TBase          Base type of the NamedType
     * \tparam TUniquePhantom Phantom type of the NamedType
     *
     * \return Base type of the NamedType
     */
    template <typename TBase, typename TUniquePhantom>
    constexpr TBase UnderlyingTypeImpl(NamedType<TBase, TUniquePhantom>);

    /**
     * \brief UnderlyingType helper method, this is used internally to get the underlying type of complex NamedType types
     * 
     * This has to be used instead of NamedType::UnderlyingType since complex NamedTypes inherits from the NamedType class.
     * This methods avoids the issue of ill-formed types.
     * 
     * \see This wonderful article, where everything is nicely explained : https://foonathan.net/blog/2016/10/19/strong-typedefs.html
     *
     * \tparam TStrongType Base type of the NamedType
     */
    template <typename TStrongType>
    using UnderlyingType = decltype(UnderlyingTypeImpl(std::declval<TStrongType>()));
}

// --- Macros

/**
 * \brief Creates a named type and a unique phantom type
 *
 * \param in_typename Name of the type to create
 * \param in_basetype Base type used
 *
 * \see NamedType
 * 
 * Creating a type using this macro allows every operation on this type.
 * This could be great for a "meter" type, but in the case of an "OGLHandle" type, this would allow nonsense like :
 *
 * Example:
 *
 * ```
 * OGLHandle handle;
 * glGenTextures(1, &handle);
 * 
 * handle /= 2; // This should not be allowed !
 * ```
 * 
 * To avoid this issue, please create a complex NamedType instead
 */
#define DAEMON_CREATE_SIMPLE_NAMED_TYPE(in_typename, in_basetype) \
    namespace internal { struct DAEMON_GLUE(Phantom, in_typename) {}; } \
    using in_typename = NamedType<in_basetype, internal::DAEMON_GLUE(Phantom, in_typename)>;

/**
 * \brief Creates a literal for a NamedType
 *
 * \param in_typename    Name of the targeted type
 * \param in_literaltype Type of the literal input
 * \param in_literalname Desired literal prefix
 * 
 * Example:
 * 
 * Calling ``DAEMON_CREATE_NAMED_TYPE_LITERAL(Test, DAEdouble, test)``
 * would allow to create test objects by writing ``12.2_test``
 */
#define DAEMON_CREATE_NAMED_TYPE_LITERAL(in_typename, in_literaltype, in_literalname) \
    constexpr in_typename operator"" DAEMON_GLUE(_, in_literalname)(in_literaltype const in_param) noexcept \
    { return in_typename(static_cast<DAEMON_GLUE(in_typename, ::UnderlyingType)>(in_param)); }

END_DAEMON_NAMESPACE