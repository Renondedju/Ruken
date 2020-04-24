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

#include <vector>

#include "Vulkan/Core/VulkanSemaphore.hpp"
#include "Vulkan/Core/VulkanTimelineSemaphore.hpp"

BEGIN_DAEMON_NAMESPACE

class SemaphorePool
{
    private:

        #pragma region Members

        DAEuint32 m_semaphore_index             {0u};
        DAEuint32 m_timeline_semaphore_index    {0u};

        std::vector<VulkanSemaphore>          m_semaphores;
        std::vector<VulkanTimelineSemaphore>  m_timeline_semaphores;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        SemaphorePool() = default;

        SemaphorePool(SemaphorePool const&  in_copy) = delete;
        SemaphorePool(SemaphorePool&&       in_move) = delete;

        ~SemaphorePool() = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        VulkanSemaphore& RequestSemaphore();

        [[nodiscard]]
        VulkanTimelineSemaphore& RequestTimelineSemaphore();

        DAEvoid Reset();

        #pragma endregion

        #pragma region Operators

        SemaphorePool& operator=(SemaphorePool const&   in_copy) = delete;
        SemaphorePool& operator=(SemaphorePool&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE