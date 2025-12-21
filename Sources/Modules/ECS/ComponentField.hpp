#pragma once

#include "Types/NonVoid.hpp"
#include "Build/Namespace.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Defines a field (or variable) for a component.
 *        In order to define a field you HAVE to inherit from this class.
 *        The name of the inheriting class will then be the way to refer to your field.
 * \warning We strongly discourage the reuse of the field across multiple components even if this is possible.
 * \warning A field can only be used once per component, doing otherwise will result in undefined behaviors
 * \tparam TDataType Type of the field
 * \tparam TComponentType Type of the component that holds this field
 */
template <NonVoid TDataType, typename TComponentType>
struct ComponentField
{
    using Type      = TDataType;
    using Component = TComponentType;
};

/**
 * \brief Declares a component field
 * \param in_field_name Name of the field class
 * \param ... Type of the field (this type is a variadic argument to support templated types with commas in them)
 */
#define RUKEN_DECLARE_FIELD(in_field_name, ...) (in_field_name, (__VA_ARGS__))

END_RUKEN_NAMESPACE