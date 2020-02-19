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

#include "Screen.hpp"
#include "Window.hpp"

#include "Containers/Vector.hpp"

#include "Debug/Logging/Logger.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief This class manages the connected screens and the created windows.
 */
class WindowManager
{
    private:

        #pragma region Members

        Logger* m_logger = nullptr;

        Vector<Window> m_windows;
        Vector<Screen> m_screens;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief This is called with an error code and a human-readable description each time a GLFW error occurs.
         *
         * \param in_error_code  An error code.
         * \param in_description A UTF-8 encoded string describing the error.
         */
        static DAEvoid ErrorCallback(DAEint32 in_error_code, DAEchar const* in_description);

        /**
         * \brief This is called when a monitor is connected to or disconnected from the system.
         *
         * \param in_monitor The monitor that was connected or disconnected.
         * \param in_event   One of 'GLFW_CONNECTED' or 'GLFW_DISCONNECTED'.
         */
        static DAEvoid MonitorCallback(GLFWmonitor* in_monitor, DAEint32 in_event);

        /**
         * \brief This is called when a joystick is connected to or disconnected from the system.
         *
         * \param in_jid   The joystick that was connected or disconnected.
         * \param in_event One of 'GLFW_CONNECTED' or 'GLFW_DISCONNECTED'.
         */
        static DAEvoid JoystickCallback(DAEint32 in_jid, DAEint32 in_event) noexcept;

        /**
         * \brief Creates a screen object managing the specified monitor.
         *
         * \param in_monitor The monitor to add.
         */
        DAEvoid AddScreen(GLFWmonitor* in_monitor);

        /**
         * \brief Destroys the screen object managing the specified monitor.
         *
         * \param in_monitor The monitor to remove.
         */
        DAEvoid RemoveScreen(GLFWmonitor* in_monitor) noexcept;

        /**
         * \brief Finds all connected monitors and creates the corresponding screen objects to manage them.
         */
        DAEvoid DiscoverScreens();

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        WindowManager() noexcept = default;

        WindowManager(WindowManager const&  in_other) = delete;
        WindowManager(WindowManager&&       in_other) = delete;

        ~WindowManager() noexcept;

        #pragma endregion

        #pragma region Operators

        WindowManager& operator=(WindowManager const&   in_other) = delete;
        WindowManager& operator=(WindowManager&&        in_other) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Initializes the GLFW library, setups the callbacks and discovers the available screens.
         *
         * \return True if the module could be initialized, else False.
         */
        DAEbool Initialize();

        /**
         * \return A pointer to the newly created window.
         */
        Window* CreateWindow();

        /**
         * \param in_window The window to destroy.
         *
         * \return True if the window could be destroyed, else False.
         */
        DAEbool DestroyWindow(Window* in_window) noexcept;

        /**
         * \return A pointer to the main window.
         */
        [[nodiscard]] Window* GetMainWindow() noexcept;

        /**
         * \return This module's logger.
         */
        [[nodiscard]] Logger* GetLogger() const noexcept;

        /**
         * \return The array of all windows.
         */
        [[nodiscard]] Vector<Window> const& GetWindows() const noexcept;

        /**
         * \return The array of all screens.
         */
        [[nodiscard]] Vector<Screen> const& GetScreens() const noexcept;

        #pragma endregion
};

/* TODO Needs to be removed when Kernel is done TODO */

extern WindowManager* GWindowManager;

/* TODO Needs to be removed when Kernel is done TODO */

END_DAEMON_NAMESPACE