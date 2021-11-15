
#pragma once

/**
 * \brief Allows to call a method on a pointer safely
 * \param in_instance instance to call the method onto
 */
#define RUKEN_SAFE_POINTER_CALL(in_instance, ...) if (in_instance != nullptr) { in_instance->__VA_ARGS__; }

/**
 * \brief Safely calls a logging instance
 * \param in_instance logger
 * \note This macro will automatically optimize out logger calls if logging has been disabled in build
 */
#if defined(RUKEN_LOGGING_ENABLED)
    #define RUKEN_SAFE_LOGGER_CALL(in_instance, ...) RUKEN_SAFE_POINTER_CALL(in_instance, __VA_ARGS__)
#else
    #define RUKEN_SAFE_LOGGER_CALL(in_instance, ...)
#endif