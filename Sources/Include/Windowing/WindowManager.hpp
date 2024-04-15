
#pragma once

#include "Meta/Meta.hpp"
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

        #pragma region Members

        // Static name of the service, used by the kernel to report service errors
        constexpr static const RkChar* service_name = RUKEN_STRING(WindowManager);

        #pragma endregion

        #pragma region Events

        Event<Window&> on_window_created   {};
        Event<Window&> on_window_destroyed {};

        #pragma endregion

        #pragma region Constructors

        WindowManager(ServiceProvider& in_service_provider) noexcept;
        
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