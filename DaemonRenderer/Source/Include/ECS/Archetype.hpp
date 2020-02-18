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

#include <tuple>

#include "Config.hpp"
#include "Types/FundamentalTypes.hpp"

#include "ECS/EntityID.hpp"
#include "ECS/ArchetypeBase.hpp"

#include "Meta/ValueIndexer.hpp"
#include "Meta/IndexSequence/QuicksortIndexSequence.hpp"

BEGIN_DAEMON_NAMESPACE

template <typename... TComponents>
class Archetype: public ArchetypeBase
{
    private:

        #pragma region Members

        std::tuple<TComponents...> m_components;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Create entity helper 
         * \tparam TIds Indices of the tuple elements
         * \return New entity id
         */
        template<DAEsize... TIds>
        EntityID CreateEntityHelper(std::index_sequence<TIds...>) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        Archetype();
        Archetype(Archetype const& in_copy) = default;
        Archetype(Archetype&&      in_move) = default;
        ~Archetype()                        = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the first occurence of the component in the archetype
         * \tparam TComponent Component to look for
         * \return First component storage occurence
         */
        template<typename TComponent>
        auto GetComponent() noexcept;

        /**
         * \brief Returns the component storage at the `TIndex` position of the archetype
         * \tparam TIndex Index to look for
         * \return Component storage reference
         */
        template<DAEsize TIndex>
        auto GetComponent() noexcept;

        /**
         * \brief Creates an entity in the archetype
         * \return The new ID of this entity.
         * \see EntityID for lifetime info
         */
        EntityID CreateEntity() noexcept;

        /**
         * \brief Returns the total count of entity stored in this archetype
         * \return Entities count
         */
        DAEsize EntitiesCount() const noexcept;

        #pragma endregion

        #pragma region Operators

        Archetype& operator=(Archetype const& in_copy) = default;
        Archetype& operator=(Archetype&&      in_move) = default;

        #pragma endregion
};

namespace internal
{
    // Hidden template magic used to make the "MakeArchetype" work

    template <std::size_t TIndex, typename... TComponents>
    using ComponentIndexerT = typename decltype(Select<TIndex>(
        Indexer<std::index_sequence<TComponents::id...>, TComponents...>{}
    ))::Type;

    template <std::size_t TLhs, std::size_t TRhs>
    struct LessComparator : std::integral_constant<DAEbool, (TLhs < TRhs)>
    {};

    template <typename TTuple, typename TSequence>
    struct ArchetypeFactory;

    template <template <typename...> class TTuple, std::size_t... TIds, typename... TComponents>
    struct ArchetypeFactory<TTuple<TComponents...>, std::index_sequence<TIds...>>
    {
        using Type = Archetype<internal::ComponentIndexerT<TIds, TComponents...>...>;
    };
}

/**
 * \brief Creates an archetype by reordering components based on their id.
 *        This makes sure that the only one archetype type is used per component combination.
 *
 * \tparam TComponents Components of the archetype to create
 */
template <typename... TComponents>
using MakeArchetype = typename internal::ArchetypeFactory<std::tuple<TComponents...>, QuicksortIndexSequenceT<internal::LessComparator, std::index_sequence<TComponents::id...>>>::Type;

#include "ECS/Archetype.inl"

END_DAEMON_NAMESPACE