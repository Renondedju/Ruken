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

#include "Core/Service.hpp"

#include "Rendering/RenderContext.hpp"

#include "Vulkan/Core/VulkanDevice.hpp"
#include "Vulkan/Core/VulkanInstance.hpp"
#include "Vulkan/Core/VulkanPhysicalDevice.hpp"

#include "Vulkan/Utilities/VulkanDeviceAllocator.hpp"

BEGIN_DAEMON_NAMESPACE

class Logger;
class Window;
class Scheduler;

class Renderer final : public Service<Renderer>
{
    private:

        #pragma region Members

        Logger*    m_logger    {nullptr};
        Scheduler* m_scheduler {nullptr};

        std::unique_ptr<VulkanInstance>        m_instance         {};
        std::unique_ptr<VulkanPhysicalDevice>  m_physical_device  {};
        std::unique_ptr<VulkanDevice>          m_device           {};
        std::unique_ptr<VulkanDeviceAllocator> m_device_allocator {};
        std::vector    <RenderContext>         m_render_contexts  {};

        #pragma endregion

        #pragma region Methods

        DAEvoid MakeContext(Window& in_window) noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        explicit Renderer(ServiceProvider& in_service_provider) noexcept;

        Renderer(Renderer const& in_copy) = delete;
        Renderer(Renderer&&      in_move) = delete;

        ~Renderer() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] VulkanInstance&        GetInstance       () const noexcept;
        [[nodiscard]] VulkanPhysicalDevice&  GetPhysicalDevice () const noexcept;
        [[nodiscard]] VulkanDevice&          GetDevice         () const noexcept;
        [[nodiscard]] VulkanDeviceAllocator& GetDeviceAllocator() const noexcept;

        #pragma endregion

        #pragma region Operators

        Renderer& operator=(Renderer const& in_copy) = delete;
        Renderer& operator=(Renderer&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE