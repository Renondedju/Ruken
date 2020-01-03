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
#include "Containers/Vector.hpp"
#include "Types/FundamentalTypes.hpp"

#include "ECS/EntityID.hpp"
#include "ECS/ArchetypeFingerprint.hpp"

BEGIN_DAEMON_NAMESPACE

template <typename... TComponents>
class Archetype
{
    private:

        #pragma region Members

        std::tuple<TComponents...> m_components;
        ArchetypeFingerprint       m_fingerprint;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Create entity helper 
         * \tparam TItems Item types to emplace back
         * \tparam TIds Indices of the tuple elements
         * \param in_entity_items Item to emplace back
         * \return New entity id
         */
        template<typename... TItems, DAEsize... TIds>
        EntityID CreateEntityHelper(TItems&&... in_entity_items, std::index_sequence<TIds...>) noexcept;

        /**
         * \brief Create entity helper 
         * \tparam TIds Indices of the tuple elements
         * \return New entity id
         */
        template<DAEsize... TIds>
        EntityID CreateEntityHelper(std::index_sequence<TIds...>) noexcept;

        /**
         * \brief Setups the fingerprint of the archetype
         * \tparam TIds Tuple index
         */
        template<DAEsize... TIds>
        DAEvoid SetupFingerprint(std::index_sequence<TIds...>) noexcept;

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
         *
         * \param in_entity_items Items to init the new entity with
         *
         * \see EntityID for lifetime info
         */
        template <typename... TItems>
        EntityID CreateEntity(TItems&&... in_entity_items) noexcept;
        EntityID CreateEntity() noexcept;

        /**
         * \brief Returns the total count of entity stored in this archetype
         * \return Entities count
         */
        DAEsize EntitiesCount() const noexcept;

        /**
         * \brief Gets the fingerprint of the archetype
         * \return Fingerprint
         */
        ArchetypeFingerprint const& GetFingerprint() const noexcept;

        #pragma endregion

        #pragma region Operators

        Archetype& operator=(Archetype const& in_copy) = default;
        Archetype& operator=(Archetype&&      in_move) = default;

        #pragma endregion
};

#include "ECS/Archetype.inl"

END_DAEMON_NAMESPACE