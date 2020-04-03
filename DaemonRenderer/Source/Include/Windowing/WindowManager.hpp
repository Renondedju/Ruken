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

#include "Windowing/Screen.hpp"
#include "Windowing/Window.hpp"

#include "Core/Service.hpp"
#include "Types/Unique.hpp"
#include "Debug/Logging/Logger.hpp"

BEGIN_DAEMON_NAMESPACE

class Logger;

/**
 * \brief Manages the connected screens and the created windows.
 */
class WindowManager final: public Service<WindowManager>, Unique
{
    private:
    
        #pragma region Members

        Logger* m_logger;

        std::vector<std::unique_ptr<Window>> m_windows;
        std::vector<std::unique_ptr<Screen>> m_screens;

        #pragma endregion

        #pragma region Methods

        #pragma region Callbacks

        /**
         * \param in_error_code  An error code.
         * \param in_description A UTF-8 encoded string describing the error.
         */
        static DAEvoid ErrorCallback(DAEint32 in_error_code, DAEchar const* in_description);

        /**
         * \param in_monitor The monitor that was connected or disconnected.
         * \param in_event   One of 'GLFW_CONNECTED' or 'GLFW_DISCONNECTED'.
         */
        static DAEvoid MonitorCallback(GLFWmonitor* in_monitor, DAEint32 in_event);

        #pragma endregion

        /**
         * \brief Creates a screen object managing the specified monitor.
         */
        DAEvoid AddScreen(GLFWmonitor* in_monitor);

        /**
         * \brief Destroys the screen object managing the specified monitor.
         */
        DAEvoid RemoveScreen(GLFWmonitor* in_monitor) noexcept;

        /**
         * \brief Finds all connected monitors and creates the corresponding screen objects to manage them.
         */
        DAEvoid DiscoverScreens();

        #pragma endregion

    public:

        #pragma region Events

        Event<Window&> on_window_created;
        Event<Window&> on_window_destroyed;
        Event<Screen&> on_screen_created;
        Event<Screen&> on_screen_destroyed;

        using Service<WindowManager>::Service;
        
        WindowManager(WindowManager const& in_other) = delete;
        WindowManager(WindowManager&&      in_other) = delete;
        ~WindowManager() noexcept;

        #pragma endregion

        #pragma region Constructors and Destructor

        /**
         * \brief Initializes the GLFW library, setups the callbacks and discovers the available screens.
         */
        WindowManager();

        WindowManager(WindowManager const&  in_copy) = delete;
        WindowManager(WindowManager&&       in_move) = delete;

        /**
         * \brief Destroys every managed window and terminates the GLFW library.
         */
        ~WindowManager() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Processes all pending events.
         */
        DAEvoid Update() noexcept;

        /**
         * \return A pointer to the window created with the specified parameters.
         */
        Window& CreateWindow(WindowParams const& in_params);

        /**
         * \return True if the window could be destroyed, else False.
         */
        DAEbool DestroyWindow(Window const* in_window) noexcept;

        /**
         * \return This module's logger.
         */
        [[nodiscard]]
        Logger* GetLogger() const noexcept;

        /**
         * \return A pointer to the requested window.
         */
        [[nodiscard]]
        Window& GetWindow(DAEuint32 in_index) noexcept;

        /**
         * \return A pointer to the main window.
         */
        [[nodiscard]]
        Window& GetMainWindow() noexcept;

        /**
         * \return A pointer to the requested screen.
         */
        [[nodiscard]]
        Screen& GetScreen(DAEuint32 in_index) noexcept;

        #pragma endregion

        #pragma region Operators

        WindowManager& operator=(WindowManager const&   in_copy) = delete;
        WindowManager& operator=(WindowManager&&        in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE