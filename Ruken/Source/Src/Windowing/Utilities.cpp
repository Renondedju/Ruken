
#include "Windowing/Utilities.hpp"

#include <GLFW/glfw3.h>

USING_RUKEN_NAMESPACE

std::string GlfwErrorToString(RkInt32 const in_error_code) noexcept
{
    switch (in_error_code)
    {
        case GLFW_NO_ERROR:            return "GLFW_NO_ERROR";
        case GLFW_NOT_INITIALIZED:     return "GLFW_NOT_INITIALIZED";
        case GLFW_NO_CURRENT_CONTEXT:  return "GLFW_NO_CURRENT_CONTEXT";
        case GLFW_INVALID_ENUM:        return "GLFW_INVALID_ENUM";
        case GLFW_INVALID_VALUE:       return "GLFW_INVALID_VALUE";
        case GLFW_OUT_OF_MEMORY:       return "GLFW_OUT_OF_MEMORY";
        case GLFW_API_UNAVAILABLE:     return "GLFW_API_UNAVAILABLE";
        case GLFW_VERSION_UNAVAILABLE: return "GLFW_VERSION_UNAVAILABLE";
        case GLFW_PLATFORM_ERROR:      return "GLFW_PLATFORM_ERROR";
        case GLFW_FORMAT_UNAVAILABLE:  return "GLFW_FORMAT_UNAVAILABLE";
        case GLFW_NO_WINDOW_CONTEXT:   return "GLFW_NO_WINDOW_CONTEXT";

        default:
            return "GLFW_UNKNOWN_ERROR_CODE";
    }
}