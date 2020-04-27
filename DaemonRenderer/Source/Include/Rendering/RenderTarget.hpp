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

#include <memory>

#include "Vulkan/Core/VulkanImage.hpp"

BEGIN_DAEMON_NAMESPACE

class RenderTarget
{
    private:

        #pragma region Members

        std::unique_ptr<VulkanImage> m_image;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        RenderTarget() = default;

        RenderTarget(RenderTarget const&    in_copy) = delete;
        RenderTarget(RenderTarget&&         in_move) = delete;

        ~RenderTarget() = default;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]]
        VulkanImage const& GetImage() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderTarget& operator=(RenderTarget const& in_copy) = delete;
        RenderTarget& operator=(RenderTarget&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE