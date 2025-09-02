#pragma once

#include "Resources/Resource.hpp"
#include "Resources/LoadContext.hpp"
#include "Filesystem/IOJobQueue.hpp"

#include <string_view>
#include <vector>

BEGIN_RUKEN_NAMESPACE

struct ResourceLoader
{
	#pragma region Lifetime

	explicit ResourceLoader()		 	             = default;
	ResourceLoader(ResourceLoader&&) 	             = default;
	ResourceLoader(ResourceLoader const&)            = default;
	ResourceLoader& operator=(ResourceLoader&&)      = default;
	ResourceLoader& operator=(ResourceLoader const&) = default;
	virtual ~ResourceLoader()						 = default;

	#pragma endregion

	/**
	 * Loads the contents of a file and outputs a resource handle.
	 * @param in_context Load context
	 * @return Load task.
	 */
	virtual IOTask<ResourcePtr<>> Load(LoadContext const& in_context) const = 0;

	/// @returns a list of the supported file extensions.
	virtual std::vector<std::string_view> SupportedExtensions() const noexcept = 0;
};

END_RUKEN_NAMESPACE