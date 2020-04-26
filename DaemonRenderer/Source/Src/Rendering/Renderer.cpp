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

#include "Rendering/Renderer.hpp"

#include "Windowing/WindowManager.hpp"

#include "Vulkan/Core/VulkanDevice.hpp"
#include "Vulkan/Core/VulkanInstance.hpp"
#include "Vulkan/Core/VulkanPhysicalDevice.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanDeviceAllocator.hpp"

#include "Core/ServiceProvider.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Renderer::Renderer(ServiceProvider& in_service_provider): Service<Renderer>(in_service_provider),
    m_logger {in_service_provider.LocateService<Logger>()->AddChild("Rendering")},
    m_scheduler {*in_service_provider.LocateService<Scheduler>()}
{
    m_logger->SetLevel(ELogLevel::Info);

    VulkanDebug ::Initialize(*m_logger);
    VulkanLoader::Initialize();

    m_instance         = std::make_unique<VulkanInstance>       ();
    m_physical_device  = std::make_unique<VulkanPhysicalDevice> (*m_instance);
    m_device           = std::make_unique<VulkanDevice>         (m_scheduler,
                                                                 *m_physical_device);
    m_device_allocator = std::make_unique<VulkanDeviceAllocator>(*m_physical_device,
                                                                 *m_device);
}

Renderer::~Renderer() noexcept
{
    m_device->WaitIdle();

    m_render_contexts.clear();

    m_device_allocator.reset();
    m_device          .reset();
    m_physical_device .reset();
    m_instance        .reset();
}

#pragma endregion

#pragma region Methods

DAEvoid Renderer::MakeContext(Window& in_window)
{
    m_render_contexts.push_back(std::make_unique<RenderContext>(*this, m_scheduler, in_window));
}

DAEvoid Renderer::OnUpdate() noexcept
{
    for (auto const& render_context : m_render_contexts)
    {
        if (render_context->BeginFrame())
        {


            render_context->EndFrame();
        }
    }
}

Logger& Renderer::GetLogger() const noexcept
{
    return *m_logger;
}

VulkanInstance& Renderer::GetInstance() const noexcept
{
    return *m_instance;
}

VulkanPhysicalDevice& Renderer::GetPhysicalDevice() const noexcept
{
    return *m_physical_device;
}

VulkanDevice& Renderer::GetDevice() const noexcept
{
    return *m_device;
}

VulkanDeviceAllocator& Renderer::GetDeviceAllocator() const noexcept
{
    return *m_device_allocator;
}

#pragma endregion