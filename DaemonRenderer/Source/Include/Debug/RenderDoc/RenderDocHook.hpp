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

#pragma once

#include <RenderDoc/renderdoc_app.h>

#include "Config.hpp"
#include "Bitwise/Bitmask.hpp"
#include "Debug/RenderDoc/ERenderDocCaptureOption.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief Render Doc runtime API debugger
 * 
 * \see https://renderdoc.org/docs/in_application_api.html
 */
class RenderDocHook
{
    private:

        #pragma region Variables

        RENDERDOC_API_1_4_0* m_renderdoc_api;

        #pragma endregion

    public:
    
        #pragma region Constructors

        RenderDocHook();
        RenderDocHook(RenderDocHook const& in_other) = default;
        RenderDocHook(RenderDocHook&&      in_other) = default;
        ~RenderDocHook()                             = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Checks if the hook is available or not.
         * \return True if the hook is available, false otherwise.
         */
        DAEbool Available() const noexcept;

        /**
         * \brief This function returns the actual API version of the implementation returned.
         * 
         * Version numbers follow semantic versioning which means the implementation returned may have a higher minor
         * and/or patch version than requested: New patch versions are identical and backwards compatible in functionality.
         * New minor versions add new functionality in a backwards compatible way.
         * 
         * \see https://semver.org/
         * 
         * \param out_major will be filled with the major version of the implementationís version.
         * \param out_minor will be filled with the minor version of the implementationís version.
         * \param out_patch will be filled with the patch version of the implementationís version.
         */
        DAEvoid GetAPIVersion(DAEint32& out_major, DAEint32& out_minor, DAEint32& out_patch) const noexcept;

        /**
         * \brief Set one of the options for tweaking some behaviours of capturing.
         * 
         * Note that each option only takes effect from after it is set - so it is advised
         * to set these options as early as possible, ideally before any graphics API has been initialised.
         * 
         * \param in_option Specifies which capture option should be set.
         * \param in_value The unsigned integer value to set for the above option.
         * \return The function returns true if the option is valid, and the value set on the option is within valid ranges, false otherwise.
         */
        DAEbool SetCaptureOptionU32(ERenderDocCaptureOption in_option, DAEuint32 in_value) const noexcept;

        /**
         * \brief Set one of the options for tweaking some behaviours of capturing.
         * 
         * Note that each option only takes effect from after it is set - so it is advised
         * to set these options as early as possible, ideally before any graphics API has been initialised.
         * 
         * \param in_option Specifies which capture option should be set.
         * \param in_value The floating point value to set for the above option.
         * \return The function returns true if the option is valid, and the value set on the option is within valid ranges, false otherwise.
         */
        DAEbool SetCaptureOptionF32(ERenderDocCaptureOption in_option, DAEfloat  in_value) const noexcept;

        /**
         * \brief Gets the current value of one of the different options listed above in SetCaptureOption().
         * \param in_option Specifies which capture option should be retrieved.
         * \return The function returns the value of the capture option, if the option is a valid RENDERDOC_CaptureOption enum. Otherwise returns 0xffffffff.
         */
        DAEuint32 GetCaptureOptionU32(ERenderDocCaptureOption in_option) const noexcept;

        /**
         * \brief Gets the current value of one of the different options listed above in SetCaptureOptionF32().
         * \param in_option Specifies which capture option should be retrieved.
         * \return The function returns the value of the capture option, if the option is a valid RENDERDOC_CaptureOption enum. Otherwise returns -FLT_MAX.
         */
        DAEfloat GetCaptureOptionF32(ERenderDocCaptureOption in_option) const noexcept;

        /**
         * \brief This function will attempt to shut down and remove RenderDoc and its hooks from the target process.
         * 
         * It must be called as early as possible in the process, and will
         * have undefined results if any graphics API functions have been called.
         */
        DAEvoid Shutdown() const noexcept;

        /**
         * \brief This function will remove RenderDocís crash handler from the target process.
         * 
         * If you have your own crash handler that you want to handle any exceptions,
         * RenderDocís handler could interfere so it can be disabled.
         */
        DAEvoid UnloadCrashHandler() const noexcept;

        /**
         * \brief Set the template for new captures.
         * 
         * The template can either be a relative or absolute path,
         * which determines where captures will be saved and how they will be named.
         * If the path template is my_captures/example then captures saved will be e.g. my_captures/example_frame123.rdc
         * and my_captures/example_frame456.rdc. Relative paths will be saved relative to the processís current working directory.
         * The default template is in a folder controlled by the UI - initially the system temporary folder,
         * and the filename is the executableís filename.
         * 
         * \param in_path_template Specifies the capture path template to set, as UTF-8 null-terminated string.
         */
        DAEvoid SetCaptureFilePathTemplate(DAEchar const* in_path_template) const noexcept;

        /**
         * \brief Get the current capture path template, see SetCaptureFilePathTemplate().
         * \return The current capture path template as a UTF-8 null-terminated string.
         */
        DAEchar const* GetCaptureFilePathTemplate() const noexcept;

        /**
         * \brief This function returns the number of frame captures that have been made.
         * \return The number of frame captures that have been made
         */
        DAEuint32 GetNumCaptures() const noexcept;

        /**
         * \brief This function returns the details of a particular frame capture, as specified by an index from 0 to GetNumCaptures() - 1.
         * 
         * The path follows the template set in SetCaptureFilePathTemplate() so it may not be an absolute path.
         *
         * \note It is advised to call this function twice - first to obtain out_path_length so that sufficient space can be allocated.
         * \note Then again to actually retrieve the path.
         *
         * \param in_idx Specifies which capture to return the details of. Must be less than the return value of GetNumCaptures().
         * \param in_filename Optional parameter filled with the UTF-8 null-terminated path to the file.
         *                      There must be enough space in the array to contain all characters including the null terminator.
         *                      If set to NULL, nothing is written.
         * \param out_path_length Optional parameter filled with the byte length of the above filename including the null terminator. If set to NULL, nothing is written.
         * \param out_timestamp Optional parameter filled with the 64-bit timestamp of the file - equivalent to the time() system call. If set to NULL, nothing is written.
         * 
         * \return Returns true if the capture index was valid, or false if it was out of range.
         */
        DAEbool GetCapture(DAEuint32 in_idx, DAEchar* in_filename, DAEuint32* out_path_length, DAEuint64* out_timestamp) const noexcept;

        /**
         * \brief This function will trigger a capture as if the user had pressed one of the capture hotkeys.
         * 
         * The capture will be taken from the next frame presented to whichever window is considered current.
         */
        DAEvoid TriggerCapture() const noexcept;

        /**
         * \brief This function returns a value to indicate whether the RenderDoc UI is currently connected to the current process.
         * \return Returns true if the RenderDoc UI is currently connected, or false otherwise.
         */
        DAEbool IsTargetControlConnected() const noexcept;

        /**
         * \brief This function will determine the closest matching replay UI executable for the current RenderDoc module and launch it.
         * 
         * \param in_connect_target_control Should be set to 1 if the UI should immediately connect to the application.
         * \param in_cmdline Optional UTF-8 null-terminated string to be appended to the command line, e.g. a capture filename.
         *                     If this parameter is NULL, the command line will be unmodified.
         * \return If the UI was successfully launched, this function will return the PID of the new process. Otherwise it will return 0.
         */
        DAEuint32 LaunchReplayUI(DAEuint32 in_connect_target_control, DAEchar const* in_cmdline) const noexcept;

        /**
         * \brief This function will explicitly set which window is considered active.
         * 
         * The active window is the one that will be captured when the keybind to trigger a capture is pressed.
         * 
         * \param in_device Handle to the API ëdeviceÅEobject that will be set active. Must be valid.
         * \param in_window_handle Handle to the platform window handle that will be set active. Must be valid.
         * 
         * \note RENDERDOC_DevicePointer is a typedef to void *. The contents of it are API specific:
         * \note For D3D11 it must be the ID3D11Device device object.
         * \note For D3D12 it must be the ID3D12Device device object.
         * \note For OpenGL it must be the HGLRC or GLXContext context object.
         * \note For Vulkan it must be the dispatch table pointer within the VkInstance.
         * \note This is a pointer-sized value at the location pointed to by the VkInstance. This is not the actual VkInstance pointer itself.
         * \note You can use the RENDERDOC_DEVICEPOINTER_FROM_VKINSTANCE helper macro defined in the renderdoc header to obtain this pointer from any VkInstance.
         * \note RENDERDOC_WindowHandle is a typedef to void *. It is the platform specific HWND, xcb_window_t, or Xlib Window.
         */
        DAEvoid SetActiveWindow(RENDERDOC_DevicePointer in_device, RENDERDOC_WindowHandle in_window_handle) const noexcept;

        /**
         * \brief This function will immediately begin a capture for the specified device/window combination.
         * 
         * \param in_device Handle to the API ëdeviceÅEobject that will be set active. May be NULL to wildcard match.
         * \param in_window_handle Handle to the platform window handle that will be set active. May be NULL to wildcard match.
         * 
         * \note RENDERDOC_DevicePointer and RENDERDOC_WindowHandle are described above in SetActiveWindow().
         * \note device and wndHandle can either or both be set to NULL to wildcard match against active device/window combinations.
         * \note This wildcard matching can be used if the handle is difficult to obtain where frame captures are triggered.
         * \note For example if in_device is NULL but in_window_handle is set, RenderDoc will begin a capture on the first API it finds that is active on that window.
         * \note If the wildcard match has multiple possible candidates, it is not defined which will be chosen.
         * \note Wildcard matching should only be used when e.g. it is known that only one API is active on a window, or there is only one window active for a given API.
         * \note If no window has been created and all rendering is off-screen,
         * \note NULL can be specified for the window handle and the device object can be passed to select that API.
         * \note If both are set to NULL, RenderDoc will simply choose one at random so is only recommended for the case where only one is present.
         */
        DAEvoid StartFrameCapture(RENDERDOC_DevicePointer in_device, RENDERDOC_WindowHandle in_window_handle) const noexcept;

        /**
         * \brief This function returns a value to indicate whether the current frame is capturing.
         * \return Returns true if the frame is currently capturing, or false otherwise.
         */
        DAEbool IsFrameCapturing() const noexcept;

        /**
         * \brief This function will immediately end an active capture for the specified device/window combination.
         * \param in_device Handle to the API ëdeviceÅEobject that will be set active. May be NULL to wildcard match.
         * \param in_window_handle Handle to the platform window handle that will be set active. May be NULL to wildcard match.
         * 
         * \note RENDERDOC_DevicePointer and RENDERDOC_WindowHandle are described above in SetActiveWindow().
         * \note in_device and in_window_handle can either or both be set to NULL to wildcard match against active device/window combinations.
         * \note This wildcard matching can be used if the handle is difficult to obtain where frame captures are triggered.
         * \note Wildcard matching of device and wndHandle is described above in BeginFrameCapture().
         * \note There will be undefined results if there is not an active frame capture for the device/window combination.
         */
        DAEvoid EndFrameCapture(RENDERDOC_DevicePointer in_device, RENDERDOC_WindowHandle in_window_handle) const noexcept;

        /**
         * \brief This function is similar to EndFrameCapture() but the capture contents will be discarded immediately, and not processed and written to disk.
         * 
         * This will be more efficient than EndFrameCapture() if the frame capture is not needed.
         * 
         * \param in_device Handle to the API ëdeviceÅEobject that will be set active. May be NULL to wildcard match.
         * \param in_window_handle Handle to the platform window handle that will be set active. May be NULL to wildcard match.
         * 
         * \note RENDERDOC_DevicePointer and RENDERDOC_WindowHandle are described above in SetActiveWindow().
         * \note in_device and in_window_handle can either or both be set to NULL to wildcard match against active device/window combinations.
         * \note This wildcard matching can be used if the handle is difficult to obtain where frame captures are triggered.
         * \note Wildcard matching of device and wndHandle is described above in BeginFrameCapture().
         * \note There will be undefined results if there is not an active frame capture for the device/window combination.
         */
        DAEvoid DiscardFrameCapture(RENDERDOC_DevicePointer in_device, RENDERDOC_WindowHandle in_window_handle) const noexcept;

        /**
         * \brief This function will trigger multiple sequential frame captures as if the user had pressed one of the capture hotkeys before each frame.
         * 
         * The captures will be taken from the next frames presented to whichever window is considered current.
         * Each capture will be taken independently and saved to a separate file, with no reference to the other frames.
         * 
         * \param in_frame_count The number of frames to capture.
         */
        DAEvoid TriggerMultiFrameCapture(DAEuint32 in_frame_count) const noexcept;

        /**
         * \brief This function adds an arbitrary comments field to an existing capture on disk, which will then be displayed in the UI to anyone opening the capture.
         * 
         * \param in_file_path Specifies the path to the capture file to set comments in, as UTF-8 null-terminated string.
         *                       If this path is NULL or an empty string, the most recent capture file that has been created will be used.
         * \param in_comments Specifies the comments to set in the capture file, as UTF-8 null-terminated string.
         */
        DAEvoid SetCaptureFileComments(DAEchar const* in_file_path, DAEchar const* in_comments) const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderDocHook& operator=(RenderDocHook const& in_other) = default;
        RenderDocHook& operator=(RenderDocHook&&      in_other) = default;

        #pragma endregion
};

END_DAEMON_NAMESPACE