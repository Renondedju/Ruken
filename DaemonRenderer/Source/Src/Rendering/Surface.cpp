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

#include "Windowing/WindowManager.hpp"

USING_DAEMON_NAMESPACE

#pragma region Constructor and Destructor

Surface::Surface(Instance const* in_instance) noexcept :
    m_handle    { nullptr },
    m_instance  { in_instance->GetHandle() }
{
    if (glfwCreateWindowSurface(m_instance, GWindowManager->GetMainWindow()->GetHandle(), nullptr, &m_handle) == VK_SUCCESS)
    {
        GRenderer->GetLogger()->Info("Window surface created successfully.");
    }

    else
        GRenderer->GetLogger()->Fatal("Failed to create window surface!");
}

Surface::~Surface() noexcept
{
    if (m_handle)
    {
        vkDestroySurfaceKHR(m_instance, m_handle, nullptr);

        GRenderer->GetLogger()->Info("Window surface destroyed.");
    }
}

#pragma endregion

#pragma region Methods

VkSurfaceKHR Surface::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion