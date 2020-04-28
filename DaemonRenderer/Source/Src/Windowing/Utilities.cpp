/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "Windowing/Utilities.hpp"

USING_DAEMON_NAMESPACE

DAEchar const* windowing::ToString(DAEint32 const in_error_code) noexcept
{
    switch (in_error_code)
    {
        case GLFW_NO_ERROR:             return "GLFW_NO_ERROR";
        case GLFW_NOT_INITIALIZED:      return "GLFW_NOT_INITIALIZED";
        case GLFW_NO_CURRENT_CONTEXT:   return "GLFW_NO_CURRENT_CONTEXT";
        case GLFW_INVALID_ENUM:         return "GLFW_INVALID_ENUM";
        case GLFW_INVALID_VALUE:        return "GLFW_INVALID_VALUE";
        case GLFW_OUT_OF_MEMORY:        return "GLFW_OUT_OF_MEMORY";
        case GLFW_API_UNAVAILABLE:      return "GLFW_API_UNAVAILABLE";
        case GLFW_VERSION_UNAVAILABLE:  return "GLFW_VERSION_UNAVAILABLE";
        case GLFW_PLATFORM_ERROR:       return "GLFW_PLATFORM_ERROR";
        case GLFW_FORMAT_UNAVAILABLE:   return "GLFW_FORMAT_UNAVAILABLE";
        case GLFW_NO_WINDOW_CONTEXT:    return "GLFW_NO_WINDOW_CONTEXT";
        default:                        return "GLFW_UNKNOWN_ERROR_CODE";
    }
}