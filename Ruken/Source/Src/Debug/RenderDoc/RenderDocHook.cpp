
#include "Debug/RenderDoc/RenderDocHook.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Utility/WindowsOS.hpp"

USING_RUKEN_NAMESPACE

RenderDocHook::RenderDocHook():
    m_renderdoc_api {nullptr}
{
#if defined(RUKEN_OS_WINDOWS)

    // Checking if the renderdoc dll is active
    if(HMODULE const mod = GetModuleHandleA("renderdoc.dll"))
    {
        pRENDERDOC_GetAPI const RenderDoc_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(GetProcAddress(mod, "RENDERDOC_GetAPI"));
        RkBool const available = RenderDoc_GetAPI(eRENDERDOC_API_Version_1_4_0, reinterpret_cast<void **>(&m_renderdoc_api));
        
        if (!available)
            m_renderdoc_api = nullptr;
    }

#elif defined(RUKEN_OS_LINUX)

    if(RkVoid* mod = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD))
    {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(dlsym(mod, "RENDERDOC_GetAPI"));
        RkBool const available = RenderDoc_GetAPI(eRENDERDOC_API_Version_1_4_0, reinterpret_cast<void **>(&m_renderdoc_api));

        if (!available)
            m_renderdoc_api = nullptr;
    }

#elif defined(RUKEN_OS_ANDROID)

    if(RkVoid* mod = dlopen("libVkLayer_GLES_RenderDoc.so", RTLD_NOW | RTLD_NOLOAD))
    {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(dlsym(mod, "RENDERDOC_GetAPI"));
        RkBool const available = RenderDoc_GetAPI(eRENDERDOC_API_Version_1_4_0, reinterpret_cast<void **>(&m_renderdoc_api));

        if (!available)
            m_renderdoc_api = nullptr;
    }

#else
    #pragma message("Render Doc Hook not support this platform")
#endif
}

RkBool RenderDocHook::Available() const noexcept
{
    return m_renderdoc_api;
}

RkVoid RenderDocHook::GetAPIVersion(RkInt32& out_major, RkInt32& out_minor, RkInt32& out_patch) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->GetAPIVersion(&out_major, &out_minor, &out_patch);
}

RkBool RenderDocHook::SetCaptureOptionU32(ERenderDocCaptureOption const in_option, RkUint32 const in_value) const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->SetCaptureOptionU32(static_cast<RENDERDOC_CaptureOption>(in_option), in_value);
}

RkBool RenderDocHook::SetCaptureOptionF32(ERenderDocCaptureOption const in_option, RkFloat const in_value) const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->SetCaptureOptionF32(static_cast<RENDERDOC_CaptureOption>(in_option), in_value);
}

RkUint32 RenderDocHook::GetCaptureOptionU32(ERenderDocCaptureOption in_option) const noexcept
{
    if (!m_renderdoc_api)
        return 0xffffffff;

    return m_renderdoc_api->GetCaptureOptionU32(static_cast<RENDERDOC_CaptureOption>(in_option));
}

RkFloat RenderDocHook::GetCaptureOptionF32(ERenderDocCaptureOption in_option) const noexcept
{
    if (!m_renderdoc_api)
        return -FLT_MAX;

    return m_renderdoc_api->GetCaptureOptionF32(static_cast<RENDERDOC_CaptureOption>(in_option));
}

RkVoid RenderDocHook::Shutdown() const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->Shutdown();
}

RkVoid RenderDocHook::UnloadCrashHandler() const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->UnloadCrashHandler();
}

RkVoid RenderDocHook::SetCaptureFilePathTemplate(RkChar const* in_path_template) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->SetCaptureFilePathTemplate(in_path_template);
}

RkChar const* RenderDocHook::GetCaptureFilePathTemplate() const noexcept
{
    if (!m_renderdoc_api)
        return "";

    return m_renderdoc_api->GetCaptureFilePathTemplate();
}

RkUint32 RenderDocHook::GetNumCaptures() const noexcept
{
    if (!m_renderdoc_api)
        return 0;

    return m_renderdoc_api->GetNumCaptures();
}

RkBool RenderDocHook::GetCapture(RkUint32 const in_idx, RkChar* in_filename, RkUint32* out_path_length, RkUint64* out_timestamp) const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->GetCapture(in_idx, in_filename, out_path_length, out_timestamp);
}

RkVoid RenderDocHook::TriggerCapture() const noexcept
{
    if (!m_renderdoc_api)
        return;

    return m_renderdoc_api->TriggerCapture();
}

RkBool RenderDocHook::IsTargetControlConnected() const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->IsTargetControlConnected();
}

RkUint32 RenderDocHook::LaunchReplayUI(RkUint32 const in_connect_target_control, RkChar const* in_cmdline) const noexcept
{
    if (!m_renderdoc_api)
        return 0;

    return m_renderdoc_api->LaunchReplayUI(in_connect_target_control, in_cmdline);
}

RkVoid RenderDocHook::SetActiveWindow(RENDERDOC_DevicePointer const in_device, RENDERDOC_WindowHandle const in_window_handle) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->SetActiveWindow(in_device, in_window_handle);
}

RkVoid RenderDocHook::StartFrameCapture(RENDERDOC_DevicePointer const in_device, RENDERDOC_WindowHandle const in_window_handle) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->StartFrameCapture(in_device, in_window_handle);
}

RkBool RenderDocHook::IsFrameCapturing() const noexcept
{
    if (!m_renderdoc_api)
        return false;

    return m_renderdoc_api->IsFrameCapturing();
}

RkVoid RenderDocHook::EndFrameCapture(RENDERDOC_DevicePointer const in_device, RENDERDOC_WindowHandle const in_window_handle) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->EndFrameCapture(in_device, in_window_handle);
}

RkVoid RenderDocHook::DiscardFrameCapture(RENDERDOC_DevicePointer const in_device, RENDERDOC_WindowHandle const in_window_handle) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->DiscardFrameCapture(in_device, in_window_handle);
}

RkVoid RenderDocHook::TriggerMultiFrameCapture(RkUint32 const in_frame_count) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->TriggerMultiFrameCapture(in_frame_count);
}

RkVoid RenderDocHook::SetCaptureFileComments(RkChar const* in_file_path, RkChar const* in_comments) const noexcept
{
    if (!m_renderdoc_api)
        return;

    m_renderdoc_api->SetCaptureFileComments(in_file_path, in_comments);
}
