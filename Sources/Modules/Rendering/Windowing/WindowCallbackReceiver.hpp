#pragma once

#include "Core/Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

struct Window;

/**
 * Window callback receiver utility.
 * Events and values are passed down from GLFW, see docs for more info.
 */
struct WindowCallbackReceiver
{
	#pragma region Lifetime

	WindowCallbackReceiver			 ()								 = default;
	WindowCallbackReceiver			 (WindowCallbackReceiver const&) = default;
	WindowCallbackReceiver			 (WindowCallbackReceiver&&)      = default;
	WindowCallbackReceiver& operator=(WindowCallbackReceiver const&) = default;
	WindowCallbackReceiver& operator=(WindowCallbackReceiver&&)      = default;
	virtual ~WindowCallbackReceiver()								 = default;

	#pragma endregion

	#pragma region Methods

	virtual RkVoid FramebufferResizeCallback(Window& in_window, int in_width,  int in_height) {}
	virtual RkVoid MouseButtonCallback	    (Window& in_window, int in_button, int in_action, int in_mods) {}
	virtual RkVoid ScrollCallback		    (Window& in_window, double in_x_offset  , double in_y_offset) {}
	virtual RkVoid CursorPositionCallback   (Window& in_window, double in_x_position, double in_y_offset) {}
	virtual RkVoid CursorEnterExitCallback  (Window& in_window, int in_entered) {}
	virtual RkVoid KeyCallback			    (Window& in_window, int in_key, int in_scancode, int in_action, int in_modifiers) {}
	virtual RkVoid CharacterCallback		(Window& in_window, unsigned int in_character) {}
	virtual RkVoid DropCallback			    (Window& in_window, int in_path_count, const char* in_paths[]) {}

	#pragma endregion
};

END_RUKEN_NAMESPACE
