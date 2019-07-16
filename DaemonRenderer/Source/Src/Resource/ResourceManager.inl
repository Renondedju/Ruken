/*
 *  MIT License
 *
 *  Copyright (c) 2019 Basile Combet, Philippe Yi
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NON INFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

template <class TUnary_Predicate>
DAEvoid ResourceManager::GarbageCollection(TUnary_Predicate in_predicate, DAEbool const in_clear_invalid_resources) noexcept
{
	if (m_collection_mode == EGCCollectionMode::Disabled)
		return;

	// If the Resource manager is already processing some resources,
	// we won't start the garbage collection now, to avoid conflicts and performances issues
	if (m_current_operation_count.load(std::memory_order_acquire) > 0u)
		return;

	ManifestsWriteAccess access(m_manifests);

	// The loop doesn't auto increments the iterator because we might need to delete iterators while looping.
	for (auto iterator = access->begin(); iterator != access->end();)
	{
		if (in_predicate(*iterator->second) && iterator->second->data)
		{
			// Scheduling the deletion
			m_scheduler_reference.ScheduleTask([manifest = iterator->second, in_clear_invalid_resources, this] {
				InvalidateResource(manifest);

				if (in_clear_invalid_resources  && manifest->status.load(std::memory_order_acquire) == EResourceStatus::Invalid)
					delete manifest;
			});
		}

		// If clearing invalid resources has been requested and the resource is invalid:
		if (in_clear_invalid_resources && iterator->second->status.load(std::memory_order_acquire) == EResourceStatus::Invalid)
			iterator = access->erase(iterator);
		else
			++iterator;
	}
}

template <typename TResource_Type>
DAEvoid ResourceManager::LoadResource(ResourceIdentifier const& in_unique_identifier, ResourceManifest* in_manifest, ResourceLoadingDescriptor const& in_descriptor, EResourceLoadingMode const in_loading_mode) noexcept
{
	if (!in_manifest)
		return;

	{
		// Since this method is susceptible to be called from multiple threads at once,
		// this ensures that a resource doesn't gets loaded twice (or more)

		// TODO move this in a special synchronization structure to avoid slowing down the whole manager
		ManifestsWriteAccess access(m_manifests);

		if (in_manifest->status.load(std::memory_order_acquire) != EResourceStatus::Invalid)
			return;

		in_manifest->status.store(EResourceStatus::Pending, std::memory_order_release);
		in_manifest->data  .store(new TResource_Type()    , std::memory_order_release);
	}

	if (in_loading_mode == EResourceLoadingMode::Synchronous)
		return LoadingRoutine(in_manifest, in_descriptor);

	m_scheduler_reference.ScheduleTask([in_manifest, &in_descriptor, this] {
		LoadingRoutine(in_manifest, in_descriptor);
	});
}

template <typename TResource_Type>
Handle<TResource_Type> ResourceManager::RequestResource(ResourceIdentifier const& in_unique_identifier, ResourceLoadingDescriptor const& in_descriptor, EResourceLoadingMode const in_loading_mode) noexcept
{
	ResourceManifest* manifest = RequestManifest(in_unique_identifier);

	// If the resource isn't currently loaded: loading it
	if (manifest->status.load(std::memory_order_acquire) == EResourceStatus::Invalid)
		LoadResource<TResource_Type>(in_unique_identifier, manifest, in_descriptor, in_loading_mode);

	return std::move(Handle<TResource_Type>(manifest));
}

template <typename TResource_Type>
Handle<TResource_Type> ResourceManager::ReloadResource(Handle<TResource_Type> const& in_handle, EResourceLoadingMode const in_loading_mode) noexcept
{
	// Cannot reload an unloaded or invalid handle
	if (!in_handle.Available())
		return in_handle;

	if (in_loading_mode == EResourceLoadingMode::Synchronous)
		ReloadingRoutine(in_handle.m_manifest);
	else
	{
		m_scheduler_reference.ScheduleTask([&in_handle, this] {
			ReloadingRoutine(in_handle.m_manifest);
		});
	}

	return in_handle;
}

template <typename TResource_Type>
Handle<TResource_Type> ResourceManager::ReferenceResource(ResourceIdentifier const& in_unique_identifier, TResource_Type* in_resource, EResourceGCStrategy const in_strategy) noexcept
{
	ManifestsWriteAccess access(m_manifests);

	// If there is already a manifest with the target name or the resource is an empty pointer
	if (access->find(in_unique_identifier) != access->end() || !in_resource)
		return Handle<TResource_Type>(nullptr);

	// Otherwise, referencing the resource
	ResourceManifest* manifest = new ResourceManifest();
	access.Get()[in_unique_identifier] = manifest;

	manifest->gc_strategy	= in_strategy;
	manifest->data			= in_resource;
	manifest->status.store(EResourceStatus::Loaded, std::memory_order_release);

	return Handle<TResource_Type>(manifest);
}

template <typename TResource_Type>
Handle<TResource_Type> ResourceManager::ReloadResource(ResourceIdentifier const& in_unique_identifier, EResourceLoadingMode const in_loading_mode) noexcept
{
	ResourceManifest* manifest = RequestManifest(in_unique_identifier, false);
	
	// Cannot reload an unloaded or invalid manifest
	if (!manifest || manifest->status != EResourceStatus::Loaded)
		return Handle<TResource_Type>(manifest);

	if (in_loading_mode == EResourceLoadingMode::Synchronous)
		ReloadingRoutine(manifest);
	else
	{
		m_scheduler_reference.ScheduleTask([&manifest, this] {
			ReloadingRoutine(manifest);
		});
	}

	return Handle<TResource_Type>(manifest);
}
