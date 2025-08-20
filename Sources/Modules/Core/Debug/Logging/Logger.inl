#pragma once

BEGIN_RUKEN_NAMESPACE

template<typename... TArgs>
RkVoid Logger::Log(
	std::string_view const in_service_name, ELogLevel const in_level,
	std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_LOGGER_ZONES));

	Dispatch(LogRecord {
		.level			  = in_level,
		.service_provider = m_service_provider.GetName(),
		.service_name	  = in_service_name,
		.exception_ptr    = nullptr,
		.message		  = std::format(in_format_string, std::forward<TArgs>(in_args)...)
	});
}

template<typename... TArgs>
RkVoid Logger::Debug(
	std::string_view const in_service_name,
	std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept
{
	Log(in_service_name, ELogLevel::Debug, in_format_string, std::forward<TArgs>(in_args)...);
}

template<typename... TArgs>
RkVoid Logger::Info(
	std::string_view in_service_name,
	std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept
{
	Log(in_service_name, ELogLevel::Info, in_format_string, std::forward<TArgs>(in_args)...);
}

template<typename... TArgs>
RkVoid Logger::Warning(
	std::string_view in_service_name,
	std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept
{
	Log(in_service_name, ELogLevel::Warning, in_format_string, std::forward<TArgs>(in_args)...);
}

template<typename... TArgs>
RkVoid Logger::Error(
	std::string_view in_service_name,
	std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept
{
	Log(in_service_name, ELogLevel::Error, in_format_string, std::forward<TArgs>(in_args)...);
}

template<typename ... TArgs>
RkVoid Logger::Exception(
	std::string_view   const in_service_name,
	std::format_string<TArgs...> in_format_string, TArgs&&... in_args,
	std::exception_ptr const in_exception) const noexcept
{
	ZoneNamed(__tracy, static_cast<bool>(RUKEN_TRACE_SHOW_LOGGER_ZONES));

	Dispatch(LogRecord {
		.level			  = ELogLevel::Error,
		.service_provider = m_service_provider.GetName(),
		.service_name	  = in_service_name,
		.exception_ptr    = in_exception,
		.message		  = std::format(in_format_string, std::forward<TArgs>(in_args)...)
	});
}

END_RUKEN_NAMESPACE
