#pragma once

#include "Resources/ResourceManifest.hpp"

BEGIN_RUKEN_NAMESPACE

template <CResource TResource>
struct ResourceHandle
{
	#pragma region Lifetime

	explicit ResourceHandle  (ResourceManifest& in_manifest) noexcept;
	ResourceHandle			 (ResourceHandle const&) noexcept;
	ResourceHandle			 (ResourceHandle&&)      noexcept;
	ResourceHandle& operator=(ResourceHandle const&) noexcept;
	ResourceHandle& operator=(ResourceHandle&&)      noexcept;
	~ResourceHandle();

	#pragma endregion

	#pragma region Methods

	/// @brief Returns a pointer to the resource or nullptr if it isn't done loading yet.
	///
	/// ResourcePtr is actually just a std::shared_ptr. This is because resources can be reloaded.
	/// When doing so, the ResourceManager just swaps around a pointer to the new resource, but we might
	/// still be reading from the old one and cannot delete it just yet. Instead, std::shared_ptr is used
	/// to make sure cleanup is done only when everybody is done.
	///
	/// @note This operator is meant to be called periodically to make sure to point
	///		  to the most up-to date version of the resource.
	///		  Throw away the pointer as soon as you are done with the resource.
	ResourcePtr<TResource> operator*() const noexcept;

	/// @brief Returns an event that is triggered when the resource finished loading.
	[[nodiscard]] ResourceLoadEvent const& LoadEvent() const;

	#pragma endregion

	private:

		// This is a pointer for convenience but is
		// treated as a reference and should never be null.
		ResourceManifest* m_manifest;
};

END_RUKEN_NAMESPACE

#include "Resources/ResourceHandle.inl"