#pragma once

// Engine
#include "Core/JobSystem/Awaitables/Primitives/AutomaticResetEvent.hpp"

// App
#include "ECSTask.hpp"
#include "Shaders/CameraData.hpp"

/// @brief Camera controller service.
/// Implements controls for a simple full-window camera.
struct CameraController : Service, WindowCallbackReceiver
{
	constexpr static std::string_view service_name = "Camera Controller";

	#pragma region Lifetime

	explicit CameraController(ServiceProvider& in_service_provider) noexcept;
	CameraController(CameraController const&) 			 = delete;
	CameraController(CameraController&&)	  			 = delete;
	CameraController& operator=(CameraController const&) = delete;
	CameraController& operator=(CameraController&&)		 = delete;
	~CameraController() override						 = default;

	#pragma endregion

	#pragma region Methods

	/// @brief   Animates the camera rotation and position independently of input callback rates.
	/// @returns Camera data to render the current frame with.
	CameraData Update() noexcept;

	RkVoid ScrollCallback		 (Window& in_window, double in_x_offset, double in_y_offset)	override;
	RkVoid MouseButtonCallback   (Window& in_window, int in_button, int in_action, int in_mods) override;
	RkVoid CursorPositionCallback(Window& in_window, double in_x_position, double in_y_offset)  override;

	#pragma endregion

	private:

		#pragma region Methods

		RkVoid RotateAround();
		RkVoid Rotate      ();
		RkVoid Zoom		   (RkFloat in_offset);
		RkVoid Move        ();
		RkVoid Pan         ();

		#pragma endregion

		#pragma region members

		// Services
		Window*	   m_window;
		Clock*     m_clock;

		// Camera interpolation data
		Vector3m   m_current_position {};
		Quaternion m_current_rotation {};
		Vector3m   m_target_position  {};
		Quaternion m_target_rotation  {};

		// Data to be uploaded on the GPU
		CameraData m_camera_data    {{}, {}, {}};

		#pragma endregion
};

