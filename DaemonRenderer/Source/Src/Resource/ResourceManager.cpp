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

#include "Resource/ResourceManager.hpp"

USING_DAEMON_NAMESPACE

DAEvoid ResourceManager::InvalidateResource(ResourceManifest* in_manifest) noexcept
{
	if (!in_manifest || in_manifest->status.load(std::memory_order_acquire) == EResourceStatus::Invalid)
		return;

	// Clearing the data
	in_manifest->data.load(std::memory_order_acquire)->Unload(*this);
	in_manifest->status.store(EResourceStatus::Invalid, std::memory_order_release);
}

ResourceManifest* ResourceManager::RequestManifest(ResourceIdentifier const& in_unique_identifier, DAEbool const in_auto_create_manifest) noexcept
{
	ManifestsWriteAccess access(m_manifests);

	// If there is no such manifest in the map: adding a new one
	if (access->find(in_unique_identifier) != access->end())
		return access.Get()[in_unique_identifier];

	ResourceManifest* manifest = nullptr;

	// Creating a new invalid manifest.
	if (in_auto_create_manifest)
	{
		manifest = new ResourceManifest();
		access.Get()[in_unique_identifier] = manifest;
	}

	return manifest;
}

DAEvoid ResourceManager::Cleanup() noexcept
{
	ManifestsWriteAccess access(m_manifests);

	// The resources will be unloaded one by one, even if the resource manager gets deleted in the process
	for (auto& [identifier, manifest] : access.Get())
	{
		m_scheduler_reference.ScheduleTask([&] {
			InvalidateResource(manifest);

			delete manifest;
		});
	}

	access->clear();
}

ResourceManager::ResourceManager(Scheduler& in_scheduler) noexcept:
	m_manifests				{},
	m_collection_mode		{EGCCollectionMode::Automatic},
	m_scheduler_reference	{in_scheduler}
{}

ResourceManager::~ResourceManager() noexcept
{
	// If the resource manager is destroyed, we need to make sure
	// that all resources are correctly deleted from memory to avoid leaks
	Cleanup();
}

DAEvoid ResourceManager::SetGarbageCollectionMode(EGCCollectionMode const in_collection_mode) noexcept
{
	m_collection_mode = in_collection_mode;
}

DAEbool ResourceManager::UnloadResource(ResourceIdentifier const& in_identifier, EResourceLoadingMode const in_loading_mode) noexcept
{
	ResourceManifest* manifest = RequestManifest(in_identifier);

	if (!manifest || manifest->status != EResourceStatus::Loaded || manifest->gc_strategy != EResourceGCStrategy::Manual)
		return false;

	if (in_loading_mode == EResourceLoadingMode::Synchronous)
		manifest->data.load(std::memory_order_acquire)->Unload(*this);
	else
		m_scheduler_reference.ScheduleTask([&manifest, this] {
			manifest->data.load(std::memory_order_acquire)->Unload(*this);
	});

	return true;
}


