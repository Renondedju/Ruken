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

#include "Rendering/RenderSystem.hpp"

#include "Windowing/WindowManager.hpp"

#include "Vulkan/Core/Device.hpp"
#include "Vulkan/Core/Instance.hpp"

/* TODO Needs to be removed when Kernel is done TODO */

DAEMON_NAMESPACE::RenderSystem* DAEMON_NAMESPACE::GRenderSystem = nullptr;

/* TODO Needs to be removed when Kernel is done TODO */

USING_DAEMON_NAMESPACE

#pragma region Static Members



#pragma endregion

#pragma region Constructor and Destructor

RenderSystem::RenderSystem():
    m_logger {GRootLogger->AddChild("Rendering")}
{
    /* TODO Needs to be removed when Kernel is done TODO */
    GRenderSystem = this;
    /* TODO Needs to be removed when Kernel is done TODO */

    m_logger->SetLevel(ELogLevel::Info);

    if ((m_instance = std::make_unique<Instance>())->Initialize(*m_logger) &&
        (m_device   = std::make_unique<Device>  ())->Initialize(*m_instance))
    {
        GWindowManager->on_window_created += [this] (Window& in_window)
        {
            MakeContext(in_window);
        };

        m_logger->Info("RenderSystem initialized successfully.");
    }

    else
        m_logger->Fatal("Failed to initialize RenderSystem!");
}

RenderSystem::~RenderSystem() noexcept
{
    m_render_contexts.clear();

    m_device  .reset();
    m_instance.reset();
}

#pragma endregion

#pragma region Methods

DAEvoid RenderSystem::MakeContext(Window& in_window)
{
    m_render_contexts.push_back(std::make_unique<RenderContext>(*m_device, in_window));
}

DAEvoid RenderSystem::OnUpdate() noexcept
{
    for (auto const& render_context : m_render_contexts)
    {
        if (render_context->BeginFrame())
        {


            render_context->EndFrame();
        }
    }
}

Logger& RenderSystem::GetLogger() const noexcept
{
    return *m_logger;
}

Instance const& RenderSystem::GetInstance() const noexcept
{
    return *m_instance;
}

Device const& RenderSystem::GetDevice() const noexcept
{
    return *m_device;
}

#pragma endregion