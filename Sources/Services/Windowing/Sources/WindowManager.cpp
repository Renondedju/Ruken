#include "Windowing/WindowManager.hpp"
#include "Windowing/Window.hpp"
#include "Debug/Exception.hpp"

#include <glfw/glfw3.h>

USING_RUKEN_NAMESPACE

WindowManager::WindowManager(ServiceProvider& in_parent):
	Service(in_parent, typeid(WindowManager))
{
	if (s_instance_count.fetch_add(1uz, std::memory_order_relaxed) == 0uz)
		if (glfwInit() != GLFW_TRUE)
			throw Exception("Cannot initialize GLFW.");
}

WindowManager::~WindowManager()
{
	if (s_instance_count.fetch_sub(1uz, std::memory_order_relaxed) == 1uz)
		glfwTerminate();
}

Window WindowManager::CreateWindow(Vector2px const& in_size, std::string_view const in_name) const
{
	return Window(in_size, in_name);
}