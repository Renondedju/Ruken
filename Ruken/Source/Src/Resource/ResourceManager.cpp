
#include <iostream>

#include "Core/ServiceProvider.hpp"
#include "Resource/ResourceManager.hpp"
#include "Resource/ResourceLoadingDescriptor.hpp"
#include "Resource/ResourceProcessingFailure.hpp"

USING_RUKEN_NAMESPACE

RkVoid ResourceManager::LoadingRoutine(ResourceManifest* in_manifest, ResourceLoadingDescriptor const& in_descriptor)
{
    in_manifest->status.store(EResourceStatus::Processed, std::memory_order_release);

    ++m_current_operation_count;
    
    try
    {
        in_manifest->data.load(std::memory_order_acquire)->Load(*this, in_descriptor);
        in_manifest->status.store(EResourceStatus::Loaded, std::memory_order_release);

        --m_current_operation_count;

        // Successfully loaded the resource
    }

    // Something happened
    catch (ResourceProcessingFailure const& failure)
    {
        in_manifest->status.store(failure.resource_validity ? EResourceStatus::Loaded : EResourceStatus::Invalid, std::memory_order_release);

        std::cout << static_cast<std::string>(in_manifest->GetIdentifier()) << " failed to load. What: " << static_cast<std::string>(failure) << std::endl;

        --m_current_operation_count;

        // If some resource tells us that there is not enough memory,
        // we are going to try to free up some in case of a retry (because we are nice ! :D)
        if (failure.code == EResourceProcessingFailureCode::OutOfMemory && m_collection_mode == EGCCollectionMode::Automatic)
            TriggerReferenceGC();
    }
    catch(...)
    {
        --m_current_operation_count;
        throw;
    }
}

RkVoid ResourceManager::ReloadingRoutine(ResourceManifest* in_manifest)
{
    if (!in_manifest || in_manifest->status.load(std::memory_order_acquire) != EResourceStatus::Loaded)
        return;

    in_manifest->status.store(EResourceStatus::Processed, std::memory_order_release);

    ++m_current_operation_count;

    try
    {
        in_manifest->data.load(std::memory_order_acquire)->Reload(*this);
        in_manifest->status.store(EResourceStatus::Loaded, std::memory_order_release);

        --m_current_operation_count;
        
        // Successfully reloaded the resource
    }

    // Something happened
    catch (ResourceProcessingFailure const& failure)
    {
        in_manifest->status.store(failure.resource_validity ? EResourceStatus::Loaded : EResourceStatus::Invalid, std::memory_order_release);

        std::cout << static_cast<std::string>(in_manifest->GetIdentifier()) << " failed to load. What: " << static_cast<std::string>(failure) << std::endl;
        --m_current_operation_count;

        // If some resource tells us that there is not enough memory,
        // we are going to try to free up some in case of a retry (because we are nice ! :D)
        if (failure.code == EResourceProcessingFailureCode::OutOfMemory)
            TriggerReferenceGC();
    }
    catch(...)
    {
        --m_current_operation_count;
        throw;
    }
}

RkVoid ResourceManager::UnloadingRoutine(ResourceManifest* in_manifest)
{
    if (!in_manifest)
        return;

    ++m_current_operation_count;
    
    if (in_manifest->status.load(std::memory_order_acquire) == EResourceStatus::Loaded)
    {
        in_manifest->data.load(std::memory_order_acquire)->Unload(*this);
        in_manifest->status.store(EResourceStatus::Invalid, std::memory_order_release);
    }

    --m_current_operation_count;
}

RkVoid ResourceManager::InvalidateResource(ResourceManifest* in_manifest) noexcept
{
    if (!in_manifest || in_manifest->status.load(std::memory_order_acquire) == EResourceStatus::Invalid)
        return;

    UnloadingRoutine(in_manifest);
}

ResourceManifest* ResourceManager::RequestManifest(ResourceIdentifier const& in_unique_identifier, RkBool const in_auto_create_manifest) noexcept
{
    ManifestsWriteAccess access(m_manifests);

    // If there is no such manifest in the map: adding a new one
    if (access->find(in_unique_identifier) != access->end())
        return access.Get()[in_unique_identifier];

    ResourceManifest* manifest = nullptr;

    // Creating a new invalid manifest.
    if (in_auto_create_manifest)
    {
        manifest = new ResourceManifest(in_unique_identifier, nullptr, EResourceGCStrategy::ReferenceCount);
        access.Get()[in_unique_identifier] = manifest;
    }

    return manifest;
}

RkVoid ResourceManager::Cleanup() noexcept
{
    // Waiting for any pending operations to be done to avoid concurrent accesses
    while (m_current_operation_count.load(std::memory_order_acquire) > 0)
        std::this_thread::yield();

    ManifestsWriteAccess access(m_manifests);

    // The resources will be unloaded one by one, even if the resource manager gets deleted in the process
    for (auto& pair : access.Get())
    {
        m_scheduler_reference.ScheduleTask([&] {
            InvalidateResource(pair.second);
            delete pair.second;
        });
    }

    access->clear();
}

ResourceManager::ResourceManager(ServiceProvider& in_service_provider) noexcept:
    Service<ResourceManager>  {in_service_provider},
    m_manifests               {},
    m_collection_mode         {EGCCollectionMode::Automatic},
    m_scheduler_reference     {*m_service_provider.LocateService<Scheduler>()},
    m_current_operation_count {0}
{}

ResourceManager::~ResourceManager() noexcept
{
    // If the resource manager is destroyed, we need to make sure
    // that all resources are correctly deleted from memory to avoid leaks
    Cleanup();
}

RkVoid ResourceManager::TriggerSceneGC() noexcept
{
    // Clearing every invalid value in the scene garbage collection since we know that every handle is more likely to be unused now.
    // (to avoid segmentation faults)
    GarbageCollection([] (ResourceManifest const& in_manifest) {
        return in_manifest.gc_strategy.load(std::memory_order_acquire) == EResourceGCStrategy::SceneDeletion;
    }, true);
}

RkVoid ResourceManager::TriggerReferenceGC() noexcept
{
    GarbageCollection([] (ResourceManifest const& in_manifest) {
        return in_manifest.gc_strategy    .load(std::memory_order_acquire) == EResourceGCStrategy::ReferenceCount &&
               in_manifest.reference_count.load(std::memory_order_acquire) == 0;
    });    
}

RkVoid ResourceManager::SetGarbageCollectionMode(EGCCollectionMode const in_collection_mode) noexcept
{
    m_collection_mode = in_collection_mode;
}

RkBool ResourceManager::UnloadResource(ResourceIdentifier const& in_identifier, ESynchronizationMode const in_loading_mode) noexcept
{
    ResourceManifest* manifest = RequestManifest(in_identifier);

    if (!manifest || manifest->status != EResourceStatus::Loaded || manifest->gc_strategy != EResourceGCStrategy::Manual)
        return false;

    if (in_loading_mode == ESynchronizationMode::Synchronous)
        UnloadingRoutine(manifest);
    else
    {
        m_scheduler_reference.ScheduleTask([&manifest, this] {
            UnloadingRoutine(manifest);
        });
    }

    return true;
}

RkUint64 ResourceManager::GetCurrentOperationCount() const noexcept
{
    return m_current_operation_count.load(std::memory_order_acquire);
}


