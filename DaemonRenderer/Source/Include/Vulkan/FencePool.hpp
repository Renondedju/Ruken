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

#include <mutex>
#include <atomic>
#include <vector>

#include "Vulkan/Core/VulkanFence.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief High level interface managing fence allocation.
 *        Fences are cached to improve performance, but they are single use.
 *        The "Reset" function must be called before on this pool in order to reuse its fences.
 */
class FencePool
{
    private:

        #pragma region Members

        std::atomic<DAEuint32>   m_index  {0u};
        std::vector<VulkanFence> m_fences {};

        mutable std::mutex m_mutex {};

        #pragma endregion

    public:

        #pragma region Constructors

        FencePool() = default;

        FencePool(FencePool const& in_copy) = delete;
        FencePool(FencePool&&      in_move) = delete;

        ~FencePool() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \return An available single use fence from the pool or a newly allocated one.
         * \note   This function is safe to call from any thread.
         */
        [[nodiscard]]
        VulkanFence& RequestFence() noexcept;

        /**
         * \brief 
         */
        [[nodiscard]]
        DAEbool Wait() const noexcept;

        /**
         * \brief Resets all managed fences, meaning previously requested ones can be reused.
         * \note  This function must be called once at the beginning of a frame.
         */
        [[nodiscard]]
        DAEbool Reset() noexcept;

        #pragma endregion

        #pragma region Operators

        FencePool& operator=(FencePool const& in_copy) = delete;
        FencePool& operator=(FencePool&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE