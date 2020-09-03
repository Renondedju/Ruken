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

#include "Core/Service.hpp"

#include "Windowing/Screen.hpp"
#include "Windowing/Window.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;

/**
 * \brief Manages the connected screens and the created windows.
 */
class WindowManager final: public Service<WindowManager>
{
    private:

        using Service<WindowManager>::Service;

        #pragma region Members

        Logger*             m_logger  {nullptr};
        std::vector<Window> m_windows {};
        std::vector<Screen> m_screens {};

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Finds all connected monitors and creates the corresponding screen objects to manage them.
         */
        RkVoid SetupScreens() noexcept;

        #pragma endregion

    public:

        #pragma region Events

        Event<Window&> on_window_created   {};
        Event<Window&> on_window_destroyed {};

        #pragma endregion

        #pragma region Constructors

        explicit WindowManager(ServiceProvider& in_service_provider) noexcept;
        
        WindowManager(WindowManager const& in_copy) = delete;
        WindowManager(WindowManager&&      in_move) = delete;

        ~WindowManager() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Processes all pending events.
         */
        RkVoid Update() const noexcept;

        /**
         * \return A reference to the newly created window.
         */
        Window& CreateWindow(WindowParams const& in_params) noexcept;

        /**
         * \brief Destroys the specified window if it is managed by the WindowManager.
         */
        RkVoid DestroyWindow(Window const& in_window) noexcept;

        #pragma endregion

        #pragma region Operators

        WindowManager& operator=(WindowManager const& in_copy) = delete;
        WindowManager& operator=(WindowManager&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE