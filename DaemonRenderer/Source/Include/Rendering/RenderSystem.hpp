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
#include <vector>

#include "Rendering/RenderContext.hpp"

#include "Debug/Logging/Logger.hpp"

BEGIN_DAEMON_NAMESPACE

class RenderSystem
{
    private:

        #pragma region Members

        Logger* m_logger;

        std::unique_ptr<class VulkanInstance>           m_instance;
        std::unique_ptr<class VulkanPhysicalDevice>     m_physical_device;
        std::unique_ptr<class VulkanDevice>             m_device;
        std::unique_ptr<class VulkanDeviceAllocator>    m_device_allocator;
        
        std::vector<std::unique_ptr<RenderContext>> m_render_contexts;

        #pragma endregion

        #pragma region Methods

        DAEvoid MakeContext(class Window& in_window);

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        RenderSystem();

        RenderSystem(RenderSystem const&    in_copy) = delete;
        RenderSystem(RenderSystem&&         in_move) = delete;

        ~RenderSystem() noexcept;

        #pragma endregion

        #pragma region Methods

        DAEvoid OnUpdate() noexcept;

        [[nodiscard]]
        Logger& GetLogger() const noexcept;

        [[nodiscard]]
        VulkanInstance& GetInstance() const noexcept;

        [[nodiscard]]
        VulkanPhysicalDevice& GetPhysicalDevice() const noexcept;

        [[nodiscard]]
        VulkanDevice& GetDevice() const noexcept;

        [[nodiscard]]
        VulkanDeviceAllocator& GetDeviceAllocator() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderSystem& operator=(RenderSystem const& in_copy) = delete;
        RenderSystem& operator=(RenderSystem&&      in_move) = delete;

        #pragma endregion
};

/* TODO Needs to be removed when Kernel is done TODO */

extern RenderSystem* GRenderSystem;

/* TODO Needs to be removed when Kernel is done TODO */

END_DAEMON_NAMESPACE