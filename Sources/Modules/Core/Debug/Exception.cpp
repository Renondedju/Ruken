#include "Core/Debug/Exception.hpp"

USING_RUKEN_NAMESPACE

Exception::Exception(std::string&&		    	 in_what,
					 std::stacktrace	  const& in_stacktrace,
					 std::source_location const& in_source_location):
	reason			{in_what},
	stacktrace      {in_stacktrace},
	source_location {in_source_location}
{}

ErrnoException::ErrnoException(RkErrno		        const  in_error_number,
							   std::stacktrace      const& in_stacktrace,
							   std::source_location const& in_source_location):
	Exception {GetErrorString(in_error_number), in_stacktrace, in_source_location}
{}

std::string ErrnoException::GetErrorString(RkErrno const in_error_number) noexcept
{
#ifdef RUKEN_COMPILER_MSVC

	constexpr std::size_t length {256};
	std::string error_message {};
	error_message.reserve(length);

	// strerror_s is an MSVC extension
	strerror_s(error_message.data(), length, in_error_number);

	return error_message;
#else
	return std::format("Errno value {} caught", in_error_number);
#endif
}
