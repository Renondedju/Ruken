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
#include "Rendering/RenderFrame.hpp"
#include "Rendering/RenderContext.hpp"

#include "Vulkan/Fence.hpp"
#include "Vulkan/Device.hpp"
#include "Vulkan/Instance.hpp"
#include "Vulkan/Semaphore.hpp"

#include "Windowing/WindowManager.hpp"

#include "Debug/Logging/Logger.hpp"

/* TODO Needs to be removed when Kernel is done TODO */

DAEMON_NAMESPACE::Renderer* DAEMON_NAMESPACE::GRenderer = nullptr;

/* TODO Needs to be removed when Kernel is done TODO */

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Renderer::Renderer():
    m_logger {GRootLogger->AddChild("Rendering")}
{
    /* TODO Needs to be removed when Kernel is done TODO */

    GRenderer = this;

    /* TODO Needs to be removed when Kernel is done TODO */

    m_logger->SetLevel(ELogLevel::Debug);

    m_logger->propagate = true;

    if (Instance::Create(m_instance) && Device::Create(*m_instance, m_device))
    {
        GWindowManager->on_window_created += [this] (Window& in_window)
        {
            MakeContext(in_window);
        };

        m_logger->Info("Renderer initialized successfully.");
    }

    else
        m_logger->Fatal("Failed to initialize Renderer!");
}

Renderer::~Renderer() noexcept
{
    m_render_contexts.clear();
    m_device         .reset();
    m_instance       .reset();

    m_logger->Info("Renderer shut down.");
}

#pragma endregion

#pragma region Methods

DAEvoid Renderer::MakeContext(Window& in_window)
{
    m_render_contexts.push_back(std::make_unique<RenderContext>(*m_instance, *m_device, in_window));
}

Logger* Renderer::GetLogger() const noexcept
{
    return m_logger;
}

Instance const& Renderer::GetInstance() const noexcept
{
    return *m_instance;
}

Device const& Renderer::GetDevice() const noexcept
{
    return *m_device;
}

#pragma endregion