#pragma once

#include "Core/Exception.hpp"
#include "Build/Namespace.hpp"

#include <stacktrace>

/*
 * This file allows "safe" includes of the windows header by removing polluting macros
 * Note that including this file in header files should be avoided at ALL costs !
 */

#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#ifndef VC_EXTRALEAN
    #define VC_EXTRALEAN
#endif

/* --- If defined, the following flags inhibit definition of the indicated items. --- */

#define NOGDICAPMASKS     // - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#define NOVIRTUALKEYCODES // - VK_*
#define NOWINMESSAGES     // - WM_*, EM_*, LB_*, CB_*
#define NOWINSTYLES       // - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#define NOSYSMETRICS      // - SM_*
#define NOMENUS           // - MF_*
#define NOICONS           // - IDI_*
#define NOKEYSTATES       // - MK_*
#define NOSYSCOMMANDS     // - SC_*
#define NORASTEROPS       // - Binary and Tertiary raster ops
#define NOSHOWWINDOW      // - SW_*
#define OEMRESOURCE       // - OEM Resource values
#define NOATOM            // - Atom Manager routines
#define NOCLIPBOARD       // - Clipboard routines
#define NOCOLOR           // - Screen colors
#define NOCTLMGR          // - Control and Dialog routines
#define NODRAWTEXT        // - DrawText() and DT_*
#define NOGDI             // - All GDI defines and routines
#define NOKERNEL          // - All KERNEL defines and routines
#define NOUSER            // - All USER defines and routines
#define NONLS             // - All NLS defines and routines
#define NOMB              // - MB_* and MessageBox()
#define NOMEMMGR          // - GMEM_*, LMEM_*, GHND, LHND, associated routines
#define NOMETAFILE        // - typedef METAFILEPICT
#define NOMINMAX          // - Macros min(a,b) and max(a,b)
#define NOMSG             // - typedef MSG and associated routines
#define NOOPENFILE        // - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#define NOSCROLL          // - SB_* and scrolling routines
#define NOSERVICE         // - All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND           // - Sound driver routines
#define NOTEXTMETRIC      // - typedef TEXTMETRIC and associated routines
#define NOWH              // - SetWindowsHook and WH_*
#define NOWINOFFSETS      // - GWL_*, GCL_*, associated routines
#define NOCOMM            // - COMM driver routines
#define NOKANJI           // - Kanji support stuff.
#define NOHELP            // - Help engine interface.
#define NOPROFILER        // - Profiler interface.
#define NODEFERWINDOWPOS  // - DeferWindowPos routines
#define NOMCX             // - Modem Configuration Extensions

// Microsoft has rewritten the C/C++ preprocessor, but it is not enabled by default for "backwards-compatibility",
// i.e. they prefer bug-compatibility with their own products instead of portability or standard compliance.
// Because of that, when enabling the /experimental:preprocessor flag to restore that compliance, the windows header spits out some warnings
#pragma warning(push)
#pragma warning(disable:5105) //Warning C5105: macro expansion producing 'defined' has undefined behavior

#include <windows.h>

#pragma warning(pop)

// Removing a bunch of junk

#undef far
#undef near
#undef RGB
#undef min
#undef max
#undef Yield

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Contains a windows exception
 */
class WindowsException final : public Exception
{
	LPSTR m_message_buffer {nullptr};

	public:

		DWORD error_code;

		#pragma region Lifetime

		/**
		 * \brief Manual constructor
		 * \param in_error_code Error code
		 * \param in_what The reason of the failure 
		 * \param in_stacktrace The stacktrace of the code that created this exception
		 */
		explicit WindowsException(const DWORD in_error_code, const std::string_view in_what, std::stacktrace const& in_stacktrace = std::stacktrace::current()) noexcept:
			Exception  (in_what.data(), in_stacktrace),
			error_code (in_error_code)
		{}

		/**
		 * \brief This constructor automatically fetches the description of the error upon construction
		 * \param in_error_code Error code
		 * \param in_stacktrace The stacktrace of the code that created this exception
		 */
		explicit WindowsException(const DWORD in_error_code, std::stacktrace const& in_stacktrace = std::stacktrace::current()) noexcept:
			WindowsException(in_error_code, [this, &in_error_code]()
			{
			    //Ask Win32 to give us the string version of that message ID.
			    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
			    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK | FORMAT_MESSAGE_IGNORE_INSERTS,
			                   nullptr, in_error_code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), reinterpret_cast<LPSTR>(&m_message_buffer), 0, nullptr);
			            
			    return m_message_buffer;
			}(), in_stacktrace)
		{
			//Free the Win32's string's buffer after initialization
			LocalFree(m_message_buffer);
		}

		/**
		 * \brief This constructor automatically fetches the last error and description of the error upon construction
		 * \param in_stacktrace The stacktrace of the code that created this exception
		 */
		explicit WindowsException(std::stacktrace const& in_stacktrace = std::stacktrace::current()) noexcept:
			WindowsException(::GetLastError(), in_stacktrace)
		{}

		#pragma endregion
};

END_RUKEN_NAMESPACE