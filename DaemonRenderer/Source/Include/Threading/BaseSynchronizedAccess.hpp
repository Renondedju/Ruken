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

#include "Config.hpp"

BEGIN_DAEMON_NAMESPACE

template <typename TType>
class Synchronized;

/**
 * \brief Base synchronized access class, implements all common methods of the SynchronizedAccess class
 * \tparam TData Synchronized's object type
 */
template<class TData, DAEbool TWriteAccess>
class BaseSynchronizedAccess
{
    protected:

        #pragma region Variables

        Synchronized<TData>& m_synchronized;

        #pragma endregion 

    public:
    
        #pragma region Constructors

        BaseSynchronizedAccess(Synchronized<TData>& in_synchronized)     noexcept;
        BaseSynchronizedAccess()                                         noexcept = delete;
        BaseSynchronizedAccess(BaseSynchronizedAccess const& in_copy)     noexcept = default;
        BaseSynchronizedAccess(BaseSynchronizedAccess&&         in_move)     noexcept = default;
        virtual ~BaseSynchronizedAccess()                                 noexcept = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Synchronized getters
         * \return Synchronized's object content
         */
        template<typename = std::enable_if_t<TWriteAccess == true>>
        [[nodiscard]]
        TData&       Get()       noexcept;
        [[nodiscard]]
        TData const& Get() const noexcept;

        #pragma endregion

        #pragma region Operators

        template<typename = std::enable_if_t<TWriteAccess == true>>
        [[nodiscard]]
        TData&       operator*()       noexcept;
        [[nodiscard]]
        TData const& operator*() const noexcept;

        template<typename = std::enable_if_t<TWriteAccess == true>>
        [[nodiscard]]
        TData*       operator->()       noexcept;
        [[nodiscard]]
        TData const* operator->() const noexcept;

        BaseSynchronizedAccess& operator=(BaseSynchronizedAccess const& in_copy) noexcept = default;
        BaseSynchronizedAccess& operator=(BaseSynchronizedAccess&&        in_move) noexcept = default;

        #pragma endregion
};

#include "Threading/BaseSynchronizedAccess.inl"

END_DAEMON_NAMESPACE
