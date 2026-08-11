// Engine
#include "Core/Time/Clock.hpp"
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
	constexpr RkFloat position_lerp_speed {0.00060f};
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
		m_current_position,
		Matrix4x4::ModelMatrix(m_current_position, m_current_rotation, Constants<Vector3m>::one),
		Matrix4x4::PerspectiveProjectionMatrix(90_deg, m_window->GetSize().AspectRatio(), 1_cm, 10_km)
	};

	return m_camera_data;
}

RkVoid CameraController::ScrollCallback(Window& in_window, double const in_x_offset, double const in_y_offset)
{
	RkBool  const shift_pressed {glfwGetKey(in_window.GLFWHandle(), GLFW_MOD_SHIFT) == GLFW_PRESS};
	RkFloat const speed			{shift_pressed ? 5.0f : 1.0f};

	Zoom(-in_y_offset * speed);
}

RkVoid CameraController::MouseButtonCallback(Window&, int const in_button, int const in_action, int const in_mods)
{

}

RkVoid CameraController::CursorPositionCallback(Window&, double const in_x_position, double const in_y_offset)
{

}

RkVoid CameraController::Zoom(RkFloat const in_offset)
{
	Vector3m const camera_forward {m_camera_data.view * Constants<Vector4m>::forward};

	m_target_position += camera_forward * in_offset;
}
