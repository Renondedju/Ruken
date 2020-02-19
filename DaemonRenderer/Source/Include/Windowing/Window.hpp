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

#include "Config.hpp"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include "Types/FundamentalTypes.hpp"

#include "Containers/String.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief 
 */
class Window
{
    /**
     * \brief WindowManager is the only class who can create a Window.
     */
    friend class WindowManager;

    private:

        #pragma region Members

        GLFWwindow* m_handle;
        DAEbool     m_has_been_initialized;

        #pragma endregion

        #pragma region Default Constructor

        Window() noexcept;

        #pragma endregion

        #pragma region Methods

        

        #pragma endregion

    public:

        #pragma region Destructor

        Window(Window const&    in_other) noexcept = delete;
        Window(Window&&         in_other) noexcept = default;

        ~Window() noexcept;

        #pragma endregion

        #pragma region Operators

        Window& operator=(Window const& in_other) noexcept = delete;
        Window& operator=(Window&&      in_other) noexcept = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief 
         */
        DAEbool Initialize();

        #pragma endregion
};

END_DAEMON_NAMESPACE