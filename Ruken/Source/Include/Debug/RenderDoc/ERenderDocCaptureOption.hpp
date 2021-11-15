
#pragma once

#include <RenderDoc/renderdoc_app.h>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief ECaptureOption enum. Defines all the possible capture option
 */
enum class ERenderDocCaptureOption : RkUint16
{
    /**
     * \brief Allow the application to enable vsync
     * 
     * Default - enabled
     *
     * 1 - The application can enable or disable vsync at will
     * 0 - vsync is force disabled
     * 
     */
    AllowVSync = eRENDERDOC_Option_APIValidation,

    /**
     * \brief Allow the application to enable fullscreen
     * 
     * Default - enabled
     * 
     * 1 - The application can enable or disable fullscreen at will
     * 0 - fullscreen is force disabled
     */
    AllowFullscreen = eRENDERDOC_Option_AllowFullscreen,

    /**
     * \brief Record API debugging events and messages
     * 
     * Default - disabled
     * 
     * 1 - Enable built-in API debugging features and records the results into
     *     the capture, which is matched up with events on replay
     * 0 - no API debugging is forcibly enabled
     */
    APIValidation = eRENDERDOC_Option_APIValidation,

    /**
     * \brief Capture CPU callstacks for API events
     * 
     * Default - disabled
     * 
     * 1 - Enables capturing of callstacks
     * 0 - no callstacks are captured
     */
    CaptureCallstacks = eRENDERDOC_Option_CaptureCallstacks,

    /**
     * \brief When capturing CPU callstacks, only capture them from drawcalls.
     * \note  This option does nothing without the above option being enabled
     *
     * Default - disabled
     * 
     * 1 - Only captures callstacks for drawcall type API events.
     *     Ignored if CaptureCallstacks is disabled
     * 0 - Callstacks, if enabled, are captured for every event.
     */
    CaptureCallstacksOnlyDraws = eRENDERDOC_Option_CaptureCallstacksOnlyDraws,

    /**
     * \brief Specify a delay in seconds to wait for a debugger to attach, after
     * \brief creating or injecting into a process, before continuing to allow it to run.
     * 
     * 0 indicates no delay, and the process will run immediately after injection
     * 
     * Default - 0 seconds
     */
    DelayForDebugger = eRENDERDOC_Option_DelayForDebugger,

    /**
     * \brief Verify buffer access. This includes checking the memory returned by a Map() call to
     * \brief detect any out-of-bounds modification, as well as initialising buffers with undefined contents
     * \brief to a marker value to catch use of uninitialised memory.
     * 
     * \note This option is only valid for OpenGL and D3D11. Explicit APIs such as D3D12 and Vulkan do
     * \note not do the same kind of interception & checking and undefined contents are really undefined.
     * 
     * Default - disabled
     * 
     * 1 - Verify buffer access
     * 0 - No verification is performed, and overwriting bounds may cause crashes or corruption in
     *     RenderDoc.
     */
    VerifyBufferAccess = eRENDERDOC_Option_VerifyBufferAccess,

    /**
     * \brief Hooks any system API calls that create child processes, and injects
     * \brief RenderDoc into them recursively with the same options.
     * 
     * Default - disabled
     * 
     * 1 - Hooks into spawned child processes
     * 0 - Child processes are not hooked by RenderDoc
     */
    HookIntoChildren = eRENDERDOC_Option_HookIntoChildren,

    /**
     * \brief By default RenderDoc only includes resources in the final capture necessary
     * \brief for that frame, this allows you to override that behaviour.
     * 
     * Default - disabled
     * 
     * 1 - all live resources at the time of capture are included in the capture
     *     and available for inspection
     * 0 - only the resources referenced by the captured frame are included
     */
    RefAllResources = eRENDERDOC_Option_RefAllResources,

    /**
     * \brief In APIs that allow for the recording of command lists to be replayed later,
     * \brief RenderDoc may choose to not capture command lists before a frame capture is
     * \brief triggered, to reduce overheads. This means any command lists recorded once
     * \brief and replayed many times will not be available and may cause a failure to
     * \brief capture.
     * 
     * \note This is only true for APIs where multithreading is difficult or
     * \note discouraged. Newer APIs like Vulkan and D3D12 will ignore this option
     * \note and always capture all command lists since the API is heavily oriented
     * \note around it and the overheads have been reduced by API design.
     * 
     * 1 - All command lists are captured from the start of the application
     * 0 - Command lists are only captured if their recording begins during
     *     the period when a frame capture is in progress.
     */
    CaptureAllCmdLists = eRENDERDOC_Option_CaptureAllCmdLists,

    /**
     * \brief Mute API debugging output when the API validation mode option is enabled
     * 
     * Default - enabled
     * 
     * 1 - Mute any API debug messages from being displayed or passed through
     * 0 - API debugging is displayed as normal
     */
    DebugOutputMute = eRENDERDOC_Option_DebugOutputMute,

    /**
     * \brief Option to allow vendor extensions to be used even when they may be
     * \brief incompatible with RenderDoc and cause corrupted replays or crashes.
     * 
     * Default - inactive
     * 
     * No values are documented, this option should only be used when absolutely
     * necessary as directed by a RenderDoc developer.
     */
    AllowUnsupportedVendorExtensions = eRENDERDOC_Option_AllowUnsupportedVendorExtensions
};

END_RUKEN_NAMESPACE