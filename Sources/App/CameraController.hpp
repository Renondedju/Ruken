#pragma once

// Engine
#include "Core/Maths/Plane.hpp"
#include "Core/Maths/SphericalCoord.hpp"
#include "Core/Bitwise/Bitmask.hpp"

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

	RkVoid ScrollCallback		 (Window& in_window, double in_x_offset, double in_y_offset)	 override;
	RkVoid MouseButtonCallback   (Window& in_window, int in_button, int in_action, int in_mods)  override;
	RkVoid CursorPositionCallback(Window& in_window, double in_x_position, double in_y_position) override;

	#pragma endregion

	private:

		#pragma region Methods

		RkVoid RotateAroundFocus(Vector2px in_start, Vector2px in_end) noexcept;
		RkVoid Rotate			(Vector2px in_start, Vector2px in_end) noexcept;
		RkVoid Pan   			(Vector2px in_start, Vector2px in_end) noexcept;
		RkVoid Zoom  			(RkFloat in_offset) 				   noexcept;
		RkVoid ZoomFov			(RkFloat in_offset) 				   noexcept;
		RkVoid HandleMovement	()									   noexcept;

		RkVoid UpdateFocusDistance() noexcept;

		#pragma endregion

		#pragma region members

		enum struct EPositionUpdateMode { Linear, Spherical };
		enum struct EUpdateMode         { Continuous, Once, None };
		enum struct ECursorMode         { Free, PanCamera, RotateCamera, RotateCameraAroundFocus };

		// Services
		Window*	    m_window;
		Clock*      m_clock;

		// Movement
		Meters	            m_focus_distance	  	{25_m};
		Vector2px           m_screen_start_position {};
		ECursorMode         m_cursor_mode		    {ECursorMode::Free};
		EUpdateMode         m_focus_update_mode		{EUpdateMode::None};
		EPositionUpdateMode m_position_update_mode  {EPositionUpdateMode::Linear};

		// Camera interpolation data
		Radians		   m_current_fov        {90_deg};
		Radians        m_target_fov         {m_current_fov};
		Vector3m   	   m_current_position   {};
		Vector3m   	   m_target_position    {m_current_position};
		SphericalCoord m_current_rotation   {Quaternion({20_deg, 0_deg, 0_deg}) * Constants<Vector3m>::forward};
		SphericalCoord m_target_rotation    {m_current_rotation};

		// Data to be uploaded on the GPU
		CameraData m_camera_data        {{}, {}};

		// Constants & Magic numbers that feels good. Lower values are snappier.
		constexpr static RkFloat s_position_lerp_speed {0.00002f};
		constexpr static RkFloat s_rotation_lerp_speed {0.000001f};
		constexpr static RkFloat      s_fov_lerp_speed {0.0005f};
		constexpr static Radians    s_fov_scroll_speed {5_deg};
		constexpr static Plane3		    s_ground_plane {
			.origin = Constants<Vector3m>::zero,
			.normal = Constants<Vector3m>::up
		};

		#pragma endregion
};

