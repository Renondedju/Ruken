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

	// Magic numbers that feels good
	constexpr RkFloat position_lerp_speed {0.00070f};
	constexpr RkFloat rotation_lerp_speed {0.00015f};

	// 1 - Interpolating
	Seconds    const  delta_time   {m_clock->LastTickDuration()};
	RkFloat    const  t_position   {1.0f - Pow(position_lerp_speed, static_cast<RkFloat>(delta_time))};
	RkFloat    const  t_rotation   {1.0f - Pow(rotation_lerp_speed, static_cast<RkFloat>(delta_time))};
	Vector3m   const  new_position {Vector3m  ::Lerp(m_current_position, m_target_position, t_position)};
	Quaternion const  new_rotation {Quaternion::Lerp(m_current_rotation, m_target_rotation, t_rotation)};

	// 2 - Updating the actual values
	m_current_position = new_position;
	m_current_rotation = new_rotation;
	m_camera_data	   = CameraData {
		Matrix4x4::ModelMatrix(m_current_position, m_current_rotation, Constants<Vector3m>::one),
		Matrix4x4::PerspectiveProjectionMatrix(90_deg, m_window->GetSize().AspectRatio(), 1_cm, 10_km),
	};

	return m_camera_data;
}

RkVoid CameraController::ScrollCallback(Window& in_window, double const in_x_offset, double const in_y_offset)
{
	RkBool  const shift_pressed {glfwGetKey(in_window.GLFWHandle(), GLFW_MOD_SHIFT) == GLFW_PRESS};
	RkFloat const speed			{shift_pressed ? 5.0f : 1.0f};

	Zoom(-in_y_offset * speed);
}

RkVoid CameraController::MouseButtonCallback(Window& in_window, int const in_button, int const in_action, int const in_mods)
{
	// Pan camera controls
	if (in_button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (in_action == GLFW_PRESS)
		{
			glfwSetInputMode(in_window.GLFWHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			m_current_movement_mode = EMovementMode::Pan;

			RkDouble x, y {};
			glfwGetCursorPos(in_window.GLFWHandle(), &x, &y);
			m_world_start_position  = m_current_position;
			m_screen_start_position = Vector2px {
				Pixels(x),
				Pixels(y)
			};
		}
		else if (in_action == GLFW_RELEASE)
		{
			glfwSetInputMode(in_window.GLFWHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			m_current_movement_mode = EMovementMode::None;
		}
	}
}

RkVoid CameraController::CursorPositionCallback(Window&, double const in_x_position, double const in_y_position)
{
	Vector2px const cursor_position {
		Pixels(in_x_position),
		Pixels(in_y_position)
	};

	if (m_current_movement_mode == EMovementMode::Pan)
		Pan(m_screen_start_position, cursor_position);

	m_screen_start_position = cursor_position;
}

RkVoid CameraController::Zoom(RkFloat const in_offset)
{
	Vector3m const camera_forward {m_camera_data.view * Constants<Vector4m>::forward};

	m_target_position += camera_forward * in_offset;
}

RkVoid CameraController::Pan(Vector2px const in_start, Vector2px const in_end)
{
	// NDC means normalized device coordinates
	Vector2px const window_size		   {m_window->GetSize()};
	Vector2f  const ndc_start_position {VectorCast<RkFloat>(in_start) / VectorCast<RkFloat>(window_size) * 2.0f - 1.0f};
	Vector2f  const ndc_end_position   {VectorCast<RkFloat>(in_end)   / VectorCast<RkFloat>(window_size) * 2.0f - 1.0f};

	Vector4m  const near_start_pos     {VectorCast<Meters>(ndc_start_position), -1.0_m, 1.0_m};
	Vector4m  const  far_start_pos     {VectorCast<Meters>(ndc_start_position),  1.0_m, 1.0_m};
	Vector4m  const   near_end_pos     {VectorCast<Meters>(  ndc_end_position), -1.0_m, 1.0_m};
	Vector4m  const    far_end_pos     {VectorCast<Meters>(  ndc_end_position),  1.0_m, 1.0_m};

	Ray3 const start_ray {
		.origin    =  (m_camera_data.inverted_view_projection * near_start_pos).Homogenize(),
		.direction = ((m_camera_data.inverted_view_projection *  far_start_pos).Homogenize() - m_camera_data.near_plane_position).Normalized()
	};

	Ray3 const end_ray {
		.origin    =  (m_camera_data.inverted_view_projection * near_end_pos).Homogenize(),
		.direction = ((m_camera_data.inverted_view_projection *  far_end_pos).Homogenize() - m_camera_data.near_plane_position).Normalized()
	};

	Plane3 const grab_plane {
		.origin = m_camera_data.near_plane_position + m_camera_data.forward * m_camera_data.near_plane_position.Length(),
		.normal = m_camera_data.forward
	};

	// Shooting 2 rays though the near plane, to get 2 world space positions
	std::optional const start_grab_position {grab_plane.RayPlaneIntersection(start_ray)};
	std::optional const   end_grab_position {grab_plane.RayPlaneIntersection(end_ray  )};

	RUKEN_ASSERT(
		start_grab_position && end_grab_position,
		"This ray plane intersection should never fail. Your maths are probably wrong."
	);

	// Finally, projecting the start and end positions to a plane facing
	// the camera to get the displacement we need to apply.
	Vector3m  const normal    {(m_camera_data.near_plane_position - start_grab_position.value()).Normalized()};
	Vector3m  const tangent   {m_camera_data.forward};
	Vector3m  const bitangent {grab_plane.normal.Cross(tangent)};
	Matrix4x4 const plane_matrix {
		tangent.x(), bitangent.x(), normal.x(), 0.0f,
		tangent.y(), bitangent.y(), normal.y(), 0.0f,
		tangent.z(), bitangent.z(), normal.z(), 0.0f,
		m_camera_data.near_plane_position.x(), m_camera_data.near_plane_position.y(), m_camera_data.near_plane_position.z(), 1.0f
	};
/*
	Matrix4x4 const plane_matrix {
		tangent.x(), bitangent.x(), normal.x(), m_camera_data.near_plane_position.x(),
		tangent.y(), bitangent.y(), normal.y(), m_camera_data.near_plane_position.y(),
		tangent.z(), bitangent.z(), normal.z(), m_camera_data.near_plane_position.z(),
		0.0f       , 0.0f         , 0.0f      , 1.0f
    };
    */
	Matrix4x4 const inverted_plane_matrix {plane_matrix.Inverted()};

	Vector3m const a((inverted_plane_matrix * Vector4m(  end_grab_position.value(), 1.0_m)).Homogenize());
	Vector3m const b((inverted_plane_matrix * Vector4m(start_grab_position.value(), 1.0_m)).Homogenize());
    Vector3m const displacement {(plane_matrix * Vector4m(a - b, 0.0_m))};

	// Finally applying the offset
	m_target_position = m_world_start_position + (start_grab_position.value() - end_grab_position.value());
}
