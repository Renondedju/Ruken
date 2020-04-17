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

#include <exception>

#include "Vulkan/Utilities/VulkanLoader.hpp"

BEGIN_DAEMON_NAMESPACE

class VulkanException final : public std::exception
{
    public:

        #pragma region Members

        #pragma endregion

        #pragma region Constructors and Destructor

        explicit VulkanException(DAEchar const* in_message) noexcept;

        VulkanException(VulkanException const&  in_copy) = default;
        VulkanException(VulkanException&&       in_move) = default;

        ~VulkanException() = default;

        #pragma endregion

        #pragma region Methods

        #pragma endregion

        #pragma region Operators

        VulkanException& operator=(VulkanException const&   in_copy) = default;
        VulkanException& operator=(VulkanException&&        in_move) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE