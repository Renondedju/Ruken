#pragma once

#include "Core/Build/Namespace.hpp"

#include <type_traits>
#include <memory>

BEGIN_RUKEN_NAMESPACE

/// @brief The base resource data class.
struct IResourceData
{
	IResourceData()								   = default;
	IResourceData(IResourceData const&)			   = default;
	IResourceData(IResourceData&&     )			   = default;
	IResourceData& operator=(IResourceData const&) = default;
	IResourceData& operator=(IResourceData&&     ) = default;
	virtual ~IResourceData()          			   = default;
};

template <typename TType>
concept CResourceData = std::is_base_of_v<IResourceData, TType>;

/// @brief Polymorphic reference-counted pointer.
///
/// ResourcePtr is actually just a std::shared_ptr. This is because resources can be reloaded.
/// When doing so, a pointer to the new resource is just swapped with the current one, but we might
/// still be reading from the old one and cannot delete it just yet. Instead, std::shared_ptr is used
/// to make sure cleanup is done only when everybody is done.
template <CResourceData TData = IResourceData>
using ResourcePtr = std::shared_ptr<TData>;

/**
 * @brief Casts a generic resource pointer into a discrete resource pointer.
 * @param in_ptr Pointer instance to cast.
 * @return Cast pointer instance.
 */
template <CResourceData TTo, CResourceData TFrom = IResourceData>
ResourcePtr<TTo> ResourcePtrCast(const ResourcePtr<TFrom>& in_ptr)
{ return std::dynamic_pointer_cast<TTo, TFrom>(in_ptr); }

END_RUKEN_NAMESPACE
