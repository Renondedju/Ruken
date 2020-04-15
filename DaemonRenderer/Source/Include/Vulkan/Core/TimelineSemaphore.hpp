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

#pragma once

#include "Vulkan/Core/Semaphore.hpp"

BEGIN_DAEMON_NAMESPACE

class TimelineSemaphore : Semaphore
{
    public:

        #pragma region Constructors and Destructor

        TimelineSemaphore() noexcept;

        TimelineSemaphore(TimelineSemaphore const&  in_copy) = delete;
        TimelineSemaphore(TimelineSemaphore&&       in_move) noexcept;

        ~TimelineSemaphore() = default;

        #pragma endregion

        #pragma region Methods

        DAEvoid Signal(DAEuint64 in_value) const noexcept;

        DAEvoid Wait(DAEuint64 in_value) const noexcept;

        [[nodiscard]]
        DAEuint64 GetValue() const noexcept;

        #pragma endregion

        #pragma region Operators

        TimelineSemaphore& operator=(TimelineSemaphore const&   in_copy) = delete;
        TimelineSemaphore& operator=(TimelineSemaphore&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE