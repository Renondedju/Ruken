
#pragma once

#include <type_traits>

#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

template <typename, template <class...> class>
struct IsInstance : public std::false_type
{ };

/**
 * \brief Checks if a class is an instance of a templated class 
 * \tparam TInstance Instance class
 * \tparam TClass Templated base class
 */
template <typename... TInstance, template <class...> class TClass>
struct IsInstance<TClass<TInstance...>, TClass> : public std::true_type
{ };

END_RUKEN_NAMESPACE