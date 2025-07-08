#pragma once

#include "Core/Service.hpp"
#include "Core/ServiceProvider.hpp"
#include "Maths/Vector/PixelVector2.hpp"

#include <atomic>

BEGIN_RUKEN_NAMESPACE

struct Window;
struct WindowManager final: Service
{
	#pragma region Lifetime

	/**
	 * Default constructor.
	 * @param in_parent Parent provider.
	 */
	explicit WindowManager(ServiceProvider& in_parent);
	WindowManager			(const WindowManager&) = delete;
	WindowManager			(WindowManager&&     ) = delete;
	WindowManager& operator=(const WindowManager&) = delete;
	WindowManager& operator=(WindowManager&&     ) = delete;
	~WindowManager() override;

	#pragma endregion

	#pragma region Methods

	/**
	 * Creates a new window.
	 * @param in_size Initial size of the window.
	 * @param in_name Initial title of the window.
	 */
	Window CreateWindow(Vector2px const& in_size, std::string_view in_name) const;

	#pragma endregion

	private:

		#pragma region Members

		static inline std::atomic_size_t s_instance_count {0UZ};

		#pragma endregion
};

END_RUKEN_NAMESPACE