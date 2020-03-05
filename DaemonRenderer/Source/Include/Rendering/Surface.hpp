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

#include <Vulkan/vulkan.h>

#include "Types/FundamentalTypes.hpp"

BEGIN_DAEMON_NAMESPACE

class Instance;

class Surface
{
    private:

        #pragma region Members

        Instance const* m_instance;

        VkSurfaceKHR m_handle;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Surface() = delete;

        explicit Surface(Instance const* in_instance);

        Surface(Instance const&    in_copy) = delete;
        Surface(Instance&&         in_move) = delete;

        ~Surface() noexcept;

        #pragma endregion

        #pragma region Operators

        Surface& operator=(Surface const& in_copy) = delete;
        Surface& operator=(Surface&&      in_move) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \return 
         */
        [[nodiscard]] VkSurfaceKHR GetHandle() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE