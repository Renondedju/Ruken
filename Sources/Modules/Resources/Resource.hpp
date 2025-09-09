#pragma once

#include "Resources/ResourcePath.hpp"

BEGIN_RUKEN_NAMESPACE

/// @brief The base resource class.
struct Resource
{
	Resource()							 = default;
	Resource(Resource const&)			 = default;
	Resource(Resource&&     )			 = default;
	Resource& operator=(Resource const&) = default;
	Resource& operator=(Resource&&     ) = default;
	virtual ~Resource()          		 = default;
};

template <typename TType>
concept CResource = std::is_base_of_v<Resource, TType>;

/// @brief Polymorphic reference-counted pointer.
///
/// ResourcePtr is actually just a std::shared_ptr. This is because resources can be reloaded.
/// When doing so, the ResourceManager just swaps around a pointer to the new resource, but we might
/// still be reading from the old one and cannot delete it just yet. Instead, std::shared_ptr is used
/// to make sure cleanup is done only when everybody is done.
template <CResource TResource = Resource>
using ResourcePtr = std::shared_ptr<TResource>;

/**
 * Casts a generic resource pointer into a discrete resource pointer.
 * @tparam TResource Resource to cast to.
 * @param in_ptr Pointer instance to cast.
 * @return Cast pointer instance.
 */
template <CResource TResource = Resource>
ResourcePtr<TResource> ResourcePtrCast(const ResourcePtr<>& in_ptr)
{ return std::dynamic_pointer_cast<TResource, Resource>(in_ptr); }

END_RUKEN_NAMESPACE
