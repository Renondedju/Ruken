// Engine
#include "Core/Time/Clock.hpp"
#include "Core/Maths/Ray.hpp"
#include "Core/Maths/Plane.hpp"
#include "Core/Maths/Vector/VectorCast.hpp"
#include "Core/Maths/Vector/ScalarVector.hpp"
#include "Rendering/Windowing/Window.hpp"

// App
#include "CameraController.hpp"

#include <tracy/Tracy.hpp>

#include "Maths/Easing.hpp"

CameraController::CameraController(ServiceProvider& in_service_provider) noexcept:
	Service  {in_service_provider, typeid(CameraController)},
	m_window {in_service_provider.LocateService<Window>()},
	m_clock  {in_service_provider.LocateService<Clock>()}
{
	RUKEN_ASSERT(m_window != nullptr, "CameraController requires a Window");
	RUKEN_ASSERT(m_clock  != nullptr, "CameraController requires a Clock");

	m_window->AddCallbackReceiver(this);
}

CameraData CameraController::Update() noexcept
{
	ZoneScoped;

	HandleMovement();

	// 1 - Clamping targets
	m_target_fov				 = Clamp<Radians>(m_target_fov, 10_deg, 140_deg);
	m_target_rotation.angles.x() = Clamp(m_target_rotation.angles.x(),
		-Constants<Radians>::pi_2,
		 Constants<Radians>::pi_2
	);

	// 2 - Interpolation
 	RkFloat  const delta_time  {m_clock->LastTickDuration()};
	RkFloat  const t_position  {1.0f - Pow(s_position_lerp_speed, delta_time)};
	RkFloat  const t_rotation  {1.0f - Pow(s_rotation_lerp_speed, delta_time)};
	RkFloat  const t_fov       {1.0f - Pow(     s_fov_lerp_speed, delta_time)};
	RkFloat  const ratio	   {m_window->GetSize().AspectRatio()};
	Vector3m const focus_point {m_current_position + m_current_rotation.DirectionVector() * m_focus_distance};

	m_current_fov      = 				 Lerp(m_current_fov	    , m_target_fov	   , t_fov);
	m_current_rotation = SphericalCoord::Lerp(m_current_rotation, m_target_rotation, t_rotation);

	if (m_position_update_mode == EPositionUpdateMode::Linear)
		m_current_position = Vector3m::Lerp(m_current_position, m_target_position, t_position);
	else
		m_current_position = m_target_position = focus_point - m_current_rotation.DirectionVector() * m_focus_distance;

	m_camera_data = CameraData {
		Matrix4x4::ModelMatrix(m_current_position, m_current_rotation.AsQuaternion()).Inverted(),
		Matrix4x4::PerspectiveProjection(m_current_fov, ratio, 1_cm, 1_km),
	};

	// 3 - Post processing
	if (m_focus_update_mode != EUpdateMode::None)
	{
		UpdateFocusDistance();

		if (m_focus_update_mode == EUpdateMode::Once)
			m_focus_update_mode = EUpdateMode::None;
	}

	return m_camera_data;
}

RkVoid CameraController::ScrollCallback(Window& in_window, [[maybe_unused]] double const in_x_offset, double const in_y_offset)
{
	GLFWwindow* const handle		{in_window.GLFWHandle()};
	RkBool      const shift_pressed {glfwGetKey(handle, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS ||
								     glfwGetKey(handle, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS};
	RkBool      const alt_pressed   {glfwGetKey(handle, GLFW_KEY_LEFT_ALT   ) == GLFW_PRESS ||
								     glfwGetKey(handle, GLFW_KEY_RIGHT_ALT  ) == GLFW_PRESS};

	if (alt_pressed) ZoomFov(-in_y_offset * (shift_pressed ? 5.0f : 1.0f));
	else			 Zoom   ( in_y_offset * (shift_pressed ? 5.0f : 1.0f));
}

RkVoid CameraController::MouseButtonCallback(Window& in_window, int const in_button, int const in_action, int const in_mods)
{
	if (in_action == GLFW_PRESS)
	{
		m_screen_start_position = in_window.GetCursorPosition();

		// Pan camera controls
		if (in_button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			m_cursor_mode = ECursorMode::PanCamera;
			UpdateFocusDistance();
		}

		// Rotations
		if (in_button == GLFW_MOUSE_BUTTON_RIGHT)
			m_cursor_mode = in_mods == GLFW_MOD_ALT ?
				ECursorMode::RotateCameraAroundFocus :
				ECursorMode::RotateCamera;
	}
	else if (in_action == GLFW_RELEASE)
		m_cursor_mode = ECursorMode::Free;
}

RkVoid CameraController::CursorPositionCallback(Window&, double const in_x_position, double const in_y_position)
{
	Vector2px const cursor_position {
		Pixels(in_x_position),
		Pixels(in_y_position)
	};

	if (m_cursor_mode == ECursorMode::PanCamera)
		Pan   (m_screen_start_position, cursor_position);
	if (m_cursor_mode == ECursorMode::RotateCamera)
		Rotate(m_screen_start_position, cursor_position);
	if (m_cursor_mode == ECursorMode::RotateCameraAroundFocus)
		RotateAroundFocus(m_screen_start_position, cursor_position);

	m_screen_start_position = cursor_position;
}

RkVoid CameraController::Rotate(Vector2px const in_start, Vector2px const in_end) noexcept
{
	Vector2px  const window_size	    {m_window->GetSize()};
	Vector2f   const ndc_start_position {VectorCast<RkFloat>(in_start) / VectorCast<RkFloat>(window_size) * 2.0f - 1.0f};
	Vector2f   const ndc_end_position   {VectorCast<RkFloat>(in_end)   / VectorCast<RkFloat>(window_size) * 2.0f - 1.0f};
	Quaternion const inv_forward		{m_current_rotation.AsQuaternion().Inverted()};

	m_focus_update_mode    = EUpdateMode        ::None;
	m_position_update_mode = EPositionUpdateMode::Linear;
	m_target_rotation     -=
		SphericalCoord(inv_forward * m_camera_data.NDCToWorldDirection(ndc_end_position  )) -
		SphericalCoord(inv_forward * m_camera_data.NDCToWorldDirection(ndc_start_position));
}

RkVoid CameraController::RotateAroundFocus(Vector2px const in_start, Vector2px const in_end) noexcept
{
	Vector2px  const window_size		{m_window->GetSize()};
	Vector2f   const ndc_start_position {VectorCast<RkFloat>(in_start) / VectorCast<RkFloat>(window_size) * 2.0f - 1.0f};
	Vector2f   const ndc_end_position   {VectorCast<RkFloat>(in_end)   / VectorCast<RkFloat>(window_size) * 2.0f - 1.0f};
	Quaternion const inv_forward        {m_current_rotation.AsQuaternion().Inverted()};

	m_focus_update_mode    = EUpdateMode		::None;
	m_position_update_mode = EPositionUpdateMode::Spherical;
	m_target_rotation     +=
		SphericalCoord(inv_forward * m_camera_data.NDCToWorldDirection(ndc_end_position  )) -
		SphericalCoord(inv_forward * m_camera_data.NDCToWorldDirection(ndc_start_position));
}

RkVoid CameraController::Zoom(RkFloat const in_offset) noexcept
{
	m_focus_update_mode    = EUpdateMode		::Continuous;
	m_position_update_mode = EPositionUpdateMode::Linear;
	m_target_position     += m_camera_data.forward * in_offset;
}

RkVoid CameraController::ZoomFov(RkFloat const in_offset) noexcept
{
	m_focus_update_mode    = EUpdateMode		::None;
	m_position_update_mode = EPositionUpdateMode::Linear;
	m_target_fov          += s_fov_scroll_speed * in_offset;
}

RkVoid CameraController::HandleMovement() noexcept
{
	// Key polling
	GLFWwindow*  window {m_window->GLFWHandle()};
	RkBool const shift_pressed {
		glfwGetKey(window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS
	};
	RkBool const up_pressed {
		glfwGetKey(window, GLFW_KEY_UP   ) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_W    ) == GLFW_PRESS
	};
	RkBool const down_pressed {
		glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_S    ) == GLFW_PRESS
	};
	RkBool const left_pressed {
		glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_A    ) == GLFW_PRESS
	};
	RkBool const right_pressed {
		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_D    ) == GLFW_PRESS
	};

	// Velocity computation
	RkFloat const delta_time {m_clock->LastTickDuration()};
	Meters  const speed      {shift_pressed ? 30_m : 10_m};
	Vector3m      direction  {};

	if (up_pressed)    direction += Constants<Vector3m>::forward;
	if (down_pressed)  direction += Constants<Vector3m>::backward;
	if (left_pressed)  direction += Constants<Vector3m>::left;
	if (right_pressed) direction += Constants<Vector3m>::right;

	if (up_pressed || down_pressed || left_pressed || right_pressed)
	{
		m_focus_update_mode    = EUpdateMode	    ::Continuous;
		m_position_update_mode = EPositionUpdateMode::Linear;
		m_target_position     += m_camera_data.inverted_view * Vector4m(direction * speed * delta_time, 0_m);
	}
}

RkVoid CameraController::Pan(Vector2px const in_start, Vector2px const in_end) noexcept
{
	// NDC means normalized device coordinates
	Vector2px const window_size		   {m_window->GetSize()};
	Vector2f  const ndc_start_position {VectorCast<RkFloat>(in_start) / VectorCast<RkFloat>(window_size) * 2.0f - 1.0f};
	Vector2f  const ndc_end_position   {VectorCast<RkFloat>(in_end)   / VectorCast<RkFloat>(window_size) * 2.0f - 1.0f};

	Ray3 const start_ray {
		.origin    = m_camera_data.NDCToWorldPosition (Vector3f {ndc_start_position, 0.0f}),
		.direction = m_camera_data.NDCToWorldDirection(ndc_start_position)
	};

	Ray3 const end_ray {
		.origin    = m_camera_data.NDCToWorldPosition (Vector3f {ndc_end_position  , 0.0f}),
		.direction = m_camera_data.NDCToWorldDirection(ndc_end_position)
	};

	Plane3 const grab_plane {
		.origin = m_camera_data.world_position + m_camera_data.forward * m_focus_distance,
		.normal = -m_camera_data.forward
	};

	// Shooting 2 rays though the near plane, to get 2 world space positions
	std::optional const start_grab_position {grab_plane.RayPlaneIntersection(start_ray)};
	std::optional const   end_grab_position {grab_plane.RayPlaneIntersection(end_ray  )};

	if (!start_grab_position || !end_grab_position)
		return;

	// Finally applying the offset
	m_focus_update_mode    = EUpdateMode        ::None;
	m_position_update_mode = EPositionUpdateMode::Linear;
	m_target_position     += start_grab_position.value() - end_grab_position.value();
}

RkVoid CameraController::UpdateFocusDistance() noexcept
{
	m_focus_distance = s_ground_plane.RayPlaneDistance(Ray3 {
		.origin    = m_camera_data.world_position,
		.direction = m_camera_data.forward
	}).value_or(25.0_m);
}