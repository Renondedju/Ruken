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

#include "Rendering/Renderer.hpp"

#include "Debug/Logging/Logger.hpp"

/* TODO Needs to be removed when Kernel is done TODO */

DAEMON_NAMESPACE::Renderer* DAEMON_NAMESPACE::GRenderer = nullptr;

/* TODO Needs to be removed when Kernel is done TODO */

USING_DAEMON_NAMESPACE

#pragma region Constructor

Renderer::Renderer() :
    m_logger            { GRootLogger->AddChild("Rendering") },
    m_instance          { nullptr },
    m_surface           { nullptr },
    m_physical_device   { nullptr },
    m_logical_device    { nullptr }
{
    GRenderer = this;

    if ((m_instance        = std::make_unique<Instance>      ())                       ->GetHandle() != nullptr &&
        (m_surface         = std::make_unique<Surface>       (m_instance       .get()))->GetHandle() != nullptr &&
        (m_physical_device = std::make_unique<PhysicalDevice>(m_instance       .get(),
                                                              m_surface        .get()))->GetHandle() != nullptr &&
        (m_logical_device  = std::make_unique<LogicalDevice> (m_physical_device.get()))->GetHandle() != nullptr &&
        (m_swapchain       = std::make_unique<Swapchain>     (m_surface        .get(),
                                                              m_physical_device.get(),
                                                              m_logical_device .get()))->GetHandle() != nullptr)
    {
        m_logger->Info("Renderer successfully initialized.");
    }

    else
        m_logger->Fatal("Failed to initialize Renderer!");
}

Renderer::~Renderer() noexcept
{
    m_swapchain      .reset();
    m_logical_device .reset();
    m_physical_device.reset();
    m_surface        .reset();
    m_instance       .reset();

    m_logger->Info("Renderer successfully shut down.");
}

#pragma endregion

#pragma region Methods

/*
DAEvoid Renderer::CreateLogicalDevice() noexcept
{
    VkDeviceCreateInfo device_info;

    device_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext                   = nullptr;
    device_info.flags                   = 0u;
    device_info.queueCreateInfoCount    = 0u;
    device_info.pQueueCreateInfos       = nullptr;
    device_info.enabledLayerCount       = 0u;
    device_info.ppEnabledLayerNames     = nullptr;
    device_info.enabledExtensionCount   = 0u;
    device_info.ppEnabledExtensionNames = nullptr;
    device_info.pEnabledFeatures        = nullptr;

    if (vkCreateDevice(m_handle, &device_info, nullptr, &m_handle) == VK_SUCCESS)
    {
        m_logger->Info("Device created successfully.");
    }

    else
        m_logger->Fatal("Failed to create device!");
}
*/

Logger* Renderer::GetLogger() const noexcept
{
    return m_logger;
}

#pragma endregion