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

#include <string>

#include "Vulkan/Utilities/VulkanLoader.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This struct contains the parameters used when creating a new window.
 */
struct WindowParams
{
    /**
     * \brief The UTF-8 encoded window title.
     */
    std::string name = "NONE";

    /**
     * \brief The position, in screen coordinates, of the upper-left corner of the content area of the window.
     */
    VkOffset2D position = { 0, 0 };

    /**
     * \brief The size, in screen coordinates, of the content area of the window.
     */
    VkExtent2D size = { 1600, 900 };

    /**
     * \brief The opacity (or alpha) value is a positive finite number between zero and one, where zero is fully transparent and one is fully opaque.
     */
    DAEfloat opacity = 1.0f;

    /**
     * \brief Whether the window starts in full screen mode.
     */
    DAEbool fullscreen = false;

    /**
     * \brief Whether the windowed mode window will be resizable by the user. 
     */
    DAEbool resizable = true;

    /**
     * \brief Whether the windowed mode window will be initially visible.
     */
    DAEbool visible = true;

    /**
     * \brief Whether the windowed mode window will have window decorations such as a border, a close widget, etc.
     */
    DAEbool decorated = true;

    /**
     * \brief Whether the windowed mode window will be given input focus when created.
     */
    DAEbool focused = true;

    /**
     * \brief Whether the full screen window will automatically iconify and restore the previous video mode on input focus loss.
     */
    DAEbool auto_iconified = true;

    /**
     * \brief Whether the windowed mode window will be floating above other regular windows, also called topmost or always-on-top.
     */
    DAEbool floating = false;

    /**
     * \brief Whether the windowed mode window will be maximized when created.
     */
    DAEbool maximized = false;

    /**
     * \brief Whether the cursor should be centered over newly created full screen windows.
     */
    DAEbool cursor_centered = true;

    /**
     * \brief Whether the window framebuffer will be transparent.
     */
    DAEbool transparent_framebuffer = false;

    /**
     * \brief whether the window will be given input focus when shown.
     */
    DAEbool focused_on_show = true;

    /**
     * \brief Whether the window content area should be resized based on the monitor content scale of any monitor it is placed on.
     * \note  This includes the initial placement when the window is created.
     */
    DAEbool scale_to_monitor = false;
};

END_DAEMON_NAMESPACE