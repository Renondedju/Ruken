#pragma once

#include "Resources/ResourceHandle.hpp"
#include "Core/Debug/Assert.hpp"

BEGIN_RUKEN_NAMESPACE

template<CResourceData TData>
ResourceHandle<TData>::ResourceHandle(std::shared_ptr<ResourceManifest>&& in_manifest) noexcept:
   m_manifest {std::forward<std::shared_ptr<ResourceManifest>>(in_manifest)}
{}

template<CResourceData TData>
ResourceHandle<TData>::ResourceHandle() noexcept:
   m_manifest {std::make_shared<ResourceManifest>()}
{}

template<CResourceData TData>
RkVoid ResourceHandle<TData>::Exchange(ResourcePtr<TData>&& in_data) noexcept
{
   RUKEN_ASSERT(in_data != nullptr, "in_data must be a valid pointer");

   m_manifest->resource_ptr.exchange(ResourcePtrCast<ResourceData, TData>(in_data), std::memory_order_relaxed);
   m_manifest->load_event  .Trigger ();
}

template<CResourceData TData>
ResourceLoadEvent const& ResourceHandle<TData>::LoadEvent() const
{
   return m_manifest->load_event;
}

template<CResourceData TData>
ResourcePtr<TData> ResourceHandle<TData>::Current() const noexcept
{
   return ResourcePtrCast<TData, ResourceData>(m_manifest->resource_ptr.load(std::memory_order_relaxed));
}

END_RUKEN_NAMESPACE
