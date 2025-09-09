#pragma once

#include "Resources/ResourceHandle.hpp"

BEGIN_RUKEN_NAMESPACE

#pragma region Lifetime

template<CResource TResource>
ResourceHandle<TResource>::ResourceHandle(ResourceManifest& in_manifest) noexcept:
	m_manifest {&in_manifest}
{
	m_manifest->references.fetch_add(1, std::memory_order_relaxed);
}

template<CResource TResource>
ResourceHandle<TResource>::ResourceHandle(ResourceHandle const& in_other) noexcept
{
	m_manifest = in_other.m_manifest;
	m_manifest->references.fetch_add(1, std::memory_order_relaxed);
}

template<CResource TResource>
ResourceHandle<TResource>::ResourceHandle(ResourceHandle&& in_other) noexcept
{
	m_manifest = std::move(in_other.m_manifest);
	m_manifest->references.fetch_add(1, std::memory_order_relaxed);
}

template<CResource TResource>
ResourceHandle<TResource>& ResourceHandle<TResource>::operator=(ResourceHandle const& in_other) noexcept
{
	m_manifest->references.fetch_sub(1, std::memory_order_relaxed);
	m_manifest = in_other.m_manifest;
	m_manifest->references.fetch_add(1, std::memory_order_relaxed);

	return *this;
}

template<CResource TResource>
ResourceHandle<TResource>& ResourceHandle<TResource>::operator=(ResourceHandle&& in_other) noexcept
{
	m_manifest->references.fetch_sub(1, std::memory_order_relaxed);
	m_manifest = std::move(in_other.m_manifest);
	m_manifest->references.fetch_add(1, std::memory_order_relaxed);

	return *this;
}

template<CResource TResource>
ResourceHandle<TResource>::~ResourceHandle()
{
	m_manifest->references.fetch_sub(1, std::memory_order_relaxed);
}

#pragma endregion

template<CResource TResource>
ResourcePtr<TResource> ResourceHandle<TResource>::operator*() const noexcept
{
	if (m_manifest->load_event.Consumed())
		return std::dynamic_pointer_cast<TResource, Resource>(m_manifest->resource_ptr.load(std::memory_order_relaxed));

	return nullptr;
}

template<CResource TResource>
ResourceLoadEvent const& ResourceHandle<TResource>::LoadEvent() const
{
	return m_manifest->load_event;
}

END_RUKEN_NAMESPACE
