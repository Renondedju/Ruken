#pragma once

#include "Resources/ResourceManifest.hpp"

BEGIN_RUKEN_NAMESPACE

template <CResourceData TData>
struct ResourceHandle
{
	#pragma region Lifetime

	/**
	 * Constructor.
	 * @param in_manifest Manifest pointer. Must not be null.
	 */
	explicit ResourceHandle(std::shared_ptr<ResourceManifest>&& in_manifest) noexcept;
	explicit ResourceHandle()												 noexcept;
	ResourceHandle(ResourceHandle const&)			 = default;
	ResourceHandle(ResourceHandle&&     )			 = default;
	ResourceHandle& operator=(ResourceHandle const&) = default;
	ResourceHandle& operator=(ResourceHandle&&     ) = default;
	~ResourceHandle()								 = default;

	#pragma endregion

	#pragma region Methods

	/// @brief Replaces the current version of the data pointer with in_data.
	RkVoid Exchange(ResourcePtr<TData>&& in_data) noexcept;

	/// @brief Returns an event that is triggered when the resource finished loading.
	[[nodiscard]] ResourceLoadEvent const& LoadEvent() const;

	/// @brief Returns the current data pointer.
	/// @note This is meant to be called periodically to make sure to point
	///		  to the most up-to date version of the resource.
	[[nodiscard]] ResourcePtr<TData> Current() const noexcept;

	#pragma endregion

	private:

		// The manifest ownership is always dependent on the lifetime of its handles but can be stored anywhere.
		// std::shared_ptr deleter allows for this kind of flexibility.
		std::shared_ptr<ResourceManifest> m_manifest;
};

END_RUKEN_NAMESPACE

#include "Resources/ResourceHandle.inl"