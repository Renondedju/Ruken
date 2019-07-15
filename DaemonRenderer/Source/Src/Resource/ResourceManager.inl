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

template <typename TResource_Type>
DAEvoid ResourceManager::LoadResource(ResourceIdentifier const& in_unique_identifier, ResourceManifest* in_manifest, ResourceLoadingDescriptor const& in_descriptor, EResourceLoadingMode in_loading_mode) noexcept
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

	auto loading_routine = [&in_unique_identifier, &in_manifest, &in_descriptor, this]
	{
		in_manifest->status.store(EResourceStatus::Processed, std::memory_order_release);

		try
		{
			in_manifest->data.load(std::memory_order_acquire)->Load(*this, in_descriptor);
			in_manifest->status.store(EResourceStatus::Loaded, std::memory_order_release);

			// Successfully loaded the resource
		}

		// Something happened
		catch (ResourceLoadingFailure const& failure)
		{
			in_manifest->status.store(failure.resource_validity ? EResourceStatus::Loaded : EResourceStatus::Invalid, std::memory_order_release);

			std::cout << in_unique_identifier.ToString() << " failed to load. Code : " <<  EnumToString(failure.code) << " : " << failure.description << std::endl;

			// If some resource tells us that there is not enough memory,
			// we are going to try to free up some in case of a retry (because we are nice ! :D)
			if (failure.code == EResourceProcessingFailureCode::OutOfMemory)
				TriggerReferenceGC();
		}
	};

	if (in_loading_mode == EResourceLoadingMode::Synchronous)
		return loading_routine();

	m_scheduler_reference.ScheduleTask(loading_routine);
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
		return;

	if (in_loading_mode == EResourceLoadingMode::Synchronous)
		in_handle.m_manifest->data.load(std::memory_order_acquire)->Reload(*this);
	else
		m_scheduler_reference.ScheduleTask([&in_handle, this] {
			in_handle.m_manifest->data.load(std::memory_order_acquire)->Reload(*this);
	});

	return in_handle;
}

template <typename TResource_Type>
Handle<TResource_Type> ResourceManager::ReloadResource(ResourceIdentifier const& in_unique_identifier, EResourceLoadingMode const in_loading_mode) noexcept
{
	ResourceManifest* manifest = RequestManifest(in_unique_identifier, false);
	
	// Cannot reload an unloaded or invalid manifest
	if (!manifest || manifest->status != EResourceStatus::Loaded)
		return Handle<TResource_Type>(manifest);

	if (in_loading_mode == EResourceLoadingMode::Synchronous)
		manifest->data.load(std::memory_order_acquire)->Reload(*this);
	else
		m_scheduler_reference.ScheduleTask([&manifest, this] {
			manifest->data.load(std::memory_order_acquire)->Reload(*this);
	});

	return Handle<TResource_Type>(manifest);
}