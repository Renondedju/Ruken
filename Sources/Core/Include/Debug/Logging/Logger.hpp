#pragma once

#include "Meta/Meta.hpp"
#include "Core/Service.hpp"

#include "Debug/Logging/Filters/LogFilter.hpp"
#include "Debug/Logging/Handlers/LogHandler.hpp"

#include <format>
#include <string_view>
#include <forward_list>
#include <tracy/Tracy.hpp>

BEGIN_RUKEN_NAMESPACE

class Logger final: public Service
{
    ELogLevel m_level {ELogLevel::Debug};

    std::forward_list<LogHandler*>      m_handlers {};
    std::forward_list<LogFilter const*> m_filters  {};

	#pragma region Methods

	/**
	 * \brief Handles a record by passing it to all handlers
	 * associated with this logger and its parents.
	 */
	RkVoid Dispatch(LogRecord const& in_record) const noexcept;

	#pragma endregion

    public:

        // Static name of the service, used by the kernel to report service errors
        constexpr static const RkChar*   service_name = RUKEN_STRING(Logger);

        #pragma region Lifetime

		explicit Logger(ServiceProvider& in_service_provider, ELogLevel in_level = ELogLevel::Debug) noexcept;
        explicit Logger(ServiceProvider& in_service_provider, std::initializer_list<LogHandler*> in_handlers, ELogLevel in_level = ELogLevel::Debug) noexcept;
         Logger			  (Logger const& in_copy) = delete;
         Logger			  (Logger&&      in_move) = delete;
		 Logger& operator=(Logger const& in_copy) = delete;
		 Logger& operator=(Logger&&      in_move) = delete;
        ~Logger() override					      = default;

        #pragma endregion

        #pragma region Methods

	    /**
	     * Logs some data to the attached handlers.
	     * @tparam TArgs Format arguments.
	     * @param in_service_name Name of the service emitting the log.
	     * @param in_level Severity of the log.
	     * @param in_format_string Format string.
	     * @param in_args Format arguments.
	     */
	    template <typename... TArgs> RkVoid Log    (std::string_view in_service_name, ELogLevel in_level, std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept;
		template <typename... TArgs> RkVoid Debug  (std::string_view in_service_name, std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept;
		template <typename... TArgs> RkVoid Info   (std::string_view in_service_name, std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept;
		template <typename... TArgs> RkVoid Warning(std::string_view in_service_name, std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept;
		template <typename... TArgs> RkVoid Error  (std::string_view in_service_name, std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept;
		template <typename... TArgs> RkVoid Fatal  (std::string_view in_service_name, std::format_string<TArgs...> in_format_string, TArgs&&... in_args) const noexcept;

        /// @brief Adds the specified filter to this logger.
        RkVoid AddFilter(LogFilter const* in_filter) noexcept;

        /// @brief Removes the specified filter from this logger.
        RkVoid RemoveFilter(LogFilter const* in_filter) noexcept;

		/// @brief Adds the specified handler to this logger.
		RkVoid AddHandler(LogHandler* in_handler) noexcept;

		/// @brief Removes the specified handler from this logger.
		RkVoid RemoveHandler(LogHandler* in_handler) noexcept;

		/// @brief Sets the threshold for this logger to the specified level.
		RkVoid SetLevel(ELogLevel in_level) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE

#include "Debug/Logging/Logger.inl"