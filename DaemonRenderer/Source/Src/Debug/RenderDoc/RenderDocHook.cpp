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

#include "Debug/RenderDoc/RenderDocHook.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Utility/WindowsOS.hpp"

USING_DAEMON_NAMESPACE

RenderDocHook::RenderDocHook():
    m_renderdoc_api {nullptr}
{
#if defined(DAEMON_OS_WINDOWS)

    // Checking if the renderdoc dll is active
    if(HMODULE const mod = GetModuleHandleA("renderdoc.dll"))
    {
        pRENDERDOC_GetAPI const RenderDoc_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(GetProcAddress(mod, "RENDERDOC_GetAPI"));
        DAEbool const available = RenderDoc_GetAPI(eRENDERDOC_API_Version_1_4_0, reinterpret_cast<void **>(&m_renderdoc_api));
        
        if (!available)
            m_renderdoc_api = nullptr;
    }

#elif defined(DAEMON_OS_LINUX)

    if(DAEvoid* mod = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD))
    {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(dlsym(mod, "RENDERDOC_GetAPI"));
        DAEbool const available = RenderDoc_GetAPI(eRENDERDOC_API_Version_1_4_0, reinterpret_cast<void **>(&m_renderdoc_api));

        if (!available)
            m_renderdoc_api = nullptr;
    }

#elif defined(DAEMON_OS_ANDROID)

    if(DAEvoid* mod = dlopen("libVkLayer_GLES_RenderDoc.so", RTLD_NOW | RTLD_NOLOAD))
    {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(dlsym(mod, "RENDERDOC_GetAPI"));
        DAEbool const available = RenderDoc_GetAPI(eRENDERDOC_API_Version_1_4_0, reinterpret_cast<void **>(&m_renderdoc_api));

        if (!available)
            m_renderdoc_api = nullptr;
    }

#else
    #pragma message("Render Doc Hook not support this platform")
#endif
}

DAEbool RenderDocHook::Available() const noexcept
{
    return m_renderdoc_api;
}

DAEvoid RenderDocHook::GetAPIVersion(DAEint32& out_major, DAEint32& out_minor, DAEint32& out_patch) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->GetAPIVersion(&out_major, &out_minor, &out_patch);
}

DAEbool RenderDocHook::SetCaptureOptionU32(ERenderDocCaptureOption const in_option, DAEuint32 const in_value) const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->SetCaptureOptionU32(static_cast<RENDERDOC_CaptureOption>(in_option), in_value);
}

DAEbool RenderDocHook::SetCaptureOptionF32(ERenderDocCaptureOption const in_option, DAEfloat const in_value) const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->SetCaptureOptionF32(static_cast<RENDERDOC_CaptureOption>(in_option), in_value);
}

DAEuint32 RenderDocHook::GetCaptureOptionU32(ERenderDocCaptureOption in_option) const noexcept
{
    if (!m_renderdoc_api)
        return 0xffffffff;

    return m_renderdoc_api->GetCaptureOptionU32(static_cast<RENDERDOC_CaptureOption>(in_option));
}

DAEfloat RenderDocHook::GetCaptureOptionF32(ERenderDocCaptureOption in_option) const noexcept
{
    if (!m_renderdoc_api)
        return -FLT_MAX;

    return m_renderdoc_api->GetCaptureOptionF32(static_cast<RENDERDOC_CaptureOption>(in_option));
}

DAEvoid RenderDocHook::Shutdown() const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->Shutdown();
}

DAEvoid RenderDocHook::UnloadCrashHandler() const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->UnloadCrashHandler();
}

DAEvoid RenderDocHook::SetCaptureFilePathTemplate(DAEchar const* in_path_template) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->SetCaptureFilePathTemplate(in_path_template);
}

DAEchar const* RenderDocHook::GetCaptureFilePathTemplate() const noexcept
{
    if (!m_renderdoc_api)
        return "";

    return m_renderdoc_api->GetCaptureFilePathTemplate();
}

DAEuint32 RenderDocHook::GetNumCaptures() const noexcept
{
    if (!m_renderdoc_api)
        return 0;

    return m_renderdoc_api->GetNumCaptures();
}

DAEbool RenderDocHook::GetCapture(DAEuint32 const in_idx, DAEchar* in_filename, DAEuint32* out_path_length, DAEuint64* out_timestamp) const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->GetCapture(in_idx, in_filename, out_path_length, out_timestamp);
}

DAEvoid RenderDocHook::TriggerCapture() const noexcept
{
    if (!m_renderdoc_api)
        return;

    return m_renderdoc_api->TriggerCapture();
}

DAEbool RenderDocHook::IsTargetControlConnected() const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->IsTargetControlConnected();
}

DAEuint32 RenderDocHook::LaunchReplayUI(DAEuint32 const in_connect_target_control, DAEchar const* in_cmdline) const noexcept
{
    if (!m_renderdoc_api)
        return 0;

    return m_renderdoc_api->LaunchReplayUI(in_connect_target_control, in_cmdline);
}

DAEvoid RenderDocHook::SetActiveWindow(RENDERDOC_DevicePointer const in_device, RENDERDOC_WindowHandle const in_window_handle) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->SetActiveWindow(in_device, in_window_handle);
}

DAEvoid RenderDocHook::StartFrameCapture(RENDERDOC_DevicePointer const in_device, RENDERDOC_WindowHandle const in_window_handle) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->StartFrameCapture(in_device, in_window_handle);
}

DAEbool RenderDocHook::IsFrameCapturing() const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->IsFrameCapturing();
}

DAEvoid RenderDocHook::EndFrameCapture(RENDERDOC_DevicePointer const in_device, RENDERDOC_WindowHandle const in_window_handle) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->EndFrameCapture(in_device, in_window_handle);
}

DAEvoid RenderDocHook::DiscardFrameCapture(RENDERDOC_DevicePointer const in_device, RENDERDOC_WindowHandle const in_window_handle) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->DiscardFrameCapture(in_device, in_window_handle);
}

DAEvoid RenderDocHook::TriggerMultiFrameCapture(DAEuint32 const in_frame_count) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->TriggerMultiFrameCapture(in_frame_count);
}

DAEvoid RenderDocHook::SetCaptureFileComments(DAEchar const* in_file_path, DAEchar const* in_comments) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->SetCaptureFileComments(in_file_path, in_comments);
}
