
#pragma once

#include <atomic>
#include <unordered_map>

#include "Build/Namespace.hpp"

#include "Meta/Meta.hpp"
#include "Core/Service.hpp"
#include "Types/FundamentalTypes.hpp"

#include "Threading/Scheduler.hpp"
#include "Threading/Synchronized.hpp"
#include "Threading/SynchronizedAccess.hpp"
#include "Threading/ESynchronizationMode.hpp"

#include "Resource/Handle.hpp"
#include "Resource/ResourceIdentifier.hpp"
#include "Resource/Enums/EGCCollectionMode.hpp"
#include "Resource/Enums/EResourceGCStrategy.hpp"

BEGIN_RUKEN_NAMESPACE

class ResourceManager final: public Service<ResourceManager>
{
    private:

        #pragma region Variables

        // Synchronized map of all the resource manifests
        Synchronized<std::unordered_map<ResourceIdentifier, struct ResourceManifest*>> m_manifests;

        // Integrated garbage collection mode of the resource manager. 
        EGCCollectionMode m_collection_mode;

        Scheduler& m_scheduler_reference;
        
        // The actual number of resource being processed
        std::atomic<RkUint64> m_current_operation_count;

        #pragma endregion

        using ManifestsType        = decltype(m_manifests)::UnderlyingType;
        using ManifestsReadAccess  = decltype(m_manifests)::ReadAccess;
        using ManifestsWriteAccess = decltype(m_manifests)::WriteAccess;

        #pragma region Methods

        RkVoid LoadingRoutine  (struct ResourceManifest* in_manifest, class ResourceLoadingDescriptor const& in_descriptor);
        RkVoid ReloadingRoutine(struct ResourceManifest* in_manifest);
        RkVoid UnloadingRoutine(struct ResourceManifest* in_manifest);

        /**
         * \brief Invalidates a resource and tags it's corresponding resource manager for garbage collection.
         * \param in_manifest Manifest of the resource to delete
         */
        RkVoid InvalidateResource(struct ResourceManifest* in_manifest) noexcept;

        /**
         * \brief Finds or creates a resource manifest by name.
         * \param in_unique_identifier Unique identifier of the resource.
         * \param in_auto_create_manifest If set to true, this method will automatically create a manifest and assign it to the passed identifier if none has been found.
         * \return The requested manifest.
         */
        [[nodiscard]]
        struct ResourceManifest* RequestManifest(ResourceIdentifier const& in_unique_identifier, RkBool in_auto_create_manifest = true) noexcept;

        /**
         * \brief Triggers a garbage collection
         * 
         * Iterates over every resource currently loaded and junks them if the
         * passed predicate return true.
         * 
         * The signature of the predicate should be:
         * bool (*in_predicate)(ResourceManifest const& in_manifest)
         * 
         * \tparam TUnary_Predicate Type of the predicate
         * \param in_predicate Predicate
         * \param in_clear_invalid_resources Should this collection clear all the invalid resources as well
         */
        template <class TUnary_Predicate>
        RkVoid GarbageCollection(TUnary_Predicate in_predicate, RkBool in_clear_invalid_resources = false) noexcept;

        /**
         * \brief Loads a resource
         * \tparam TResource_Type Resource type to load
         * \param in_manifest Manifest to put the resource into
         * \param in_descriptor Parameters to pass to the resource loader
         * \param in_loading_mode Loading mode of the resource (async/sync)
         */
        template <typename TResource_Type>
        RkVoid LoadResource(ResourceManifest* in_manifest, class ResourceLoadingDescriptor const& in_descriptor, ESynchronizationMode in_loading_mode) noexcept;

        /**
         * \brief Unloads all the currently loaded resources in the manager
         * \warning This method also invalidates every handle, be careful
         */
        RkVoid Cleanup() noexcept;

        #pragma endregion

    public:

        #pragma region Members

        // Static name of the service, used by the kernel to report service errors
        constexpr static const RkChar* service_name = RUKEN_STRING(ResourceManager);

        #pragma endregion

        #pragma region Constructors

        ResourceManager (ServiceProvider& in_service_provider) noexcept;

        ResourceManager (ResourceManager const& in_copy) = delete;
        ResourceManager (ResourceManager&&      in_move) = delete;
        ~ResourceManager() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Triggers the scene garbage collection.
         * 
         * ie. the garbage collection of all the resources using the
         * EResourceGCStrategy::SceneDeletion strategy.
         */
        RkVoid TriggerSceneGC() noexcept;

        /**
         * \brief Triggers the garbage collection of all the resources that are using the EResourceGCStrategy::ReferenceCount strategy.
         */
        RkVoid TriggerReferenceGC() noexcept;

        /**
         * \brief References a resource into the resource manager 
         * 
         * Any resource referenced to the resource manager gets it's ownership transferred to the resource manager.
         * This means that the resource manager will now manage and apply garbage collection to the referenced resource.
         * The newly referenced resource will have to be exclusively accessed using a Handle like any other resource.
         * 
         * If the identifier already exists, the returned handle will be invalid.
         * 
         * \tparam TResource_Type Type of the resource to reference
         * \param in_unique_identifier Unique identifier of the resource
         * \param in_resource Resource to reference. This resource has to be on the stack and will now be managed by the resource manager.
         * \param in_strategy Default garbage collection strategy
         * \return Handle to the newly referenced resource.
         */
        template <typename TResource_Type>
        Handle<TResource_Type> ReferenceResource(ResourceIdentifier const& in_unique_identifier, TResource_Type* in_resource, EResourceGCStrategy in_strategy = EResourceGCStrategy::ReferenceCount) noexcept;

        /**
         * \brief Reloads a resource. This requires the resource to already be loaded and available.
         * 
         * \tparam TResource_Type Type of the resource to reload
         * \param in_unique_identifier Unique identifier of the resource
         * \param in_loading_mode Resource loading mode. See ESynchronizationMode for more detailed information.
         * \return Handle to the resource
         */
        template <typename TResource_Type>
        Handle<TResource_Type> ReloadResource(ResourceIdentifier const& in_unique_identifier, ESynchronizationMode in_loading_mode = ESynchronizationMode::Asynchronous) noexcept;

        /**
         * \brief Reloads a resource. This requires the resource to already be loaded and available.
         * 
         * Alternative overload using the resource handle instead
         * 
         * \tparam TResource_Type Type of the resource to reload
         * \param in_handle Unique identifier of the resource
         * \param in_loading_mode Resource loading mode. See ESynchronizationMode for more detailed information.
         * \return Handle to the resource
         */
        template <typename TResource_Type>
        Handle<TResource_Type> ReloadResource(Handle<TResource_Type> const& in_handle, ESynchronizationMode in_loading_mode = ESynchronizationMode::Asynchronous) noexcept;

        /**
         * \brief Requests a resource from the resource manager.
         * 
         * \tparam TResource_Type Type of the resource to request
         * \param in_unique_identifier Unique name of the resource, this identifier is the same whatever the type of the requested resource.
         * \param in_descriptor Description of the resource
         * \param in_loading_mode Resource loading mode. See ESynchronizationMode for more detailed information.
         * \return Handle to the resource
         */
        template <typename TResource_Type>
        Handle<TResource_Type> RequestResource(ResourceIdentifier const& in_unique_identifier, class ResourceLoadingDescriptor const& in_descriptor, ESynchronizationMode in_loading_mode = ESynchronizationMode::Asynchronous) noexcept;
        
        /**
         * \brief Sets the garbage collection mode of the resource manager
         * 
         * This defines how the garbage collector behaves
         * 
         * EGCCollectionMode::Automatic => Default mode. Automatic GC triggering to optimize ram usage.
         * EGCCollectionMode::Disabled  => Disabled, no GC can be triggered when this mode is active. This is useful when reloading a scene
         * EGCCollectionMode::Manual    => Manual garbage collection. Automatic GC calls are disabled.
         * 
         * \param in_collection_mode Garbage Collection mode
         */
        RkVoid SetGarbageCollectionMode(EGCCollectionMode in_collection_mode) noexcept;

        /**
         * \brief Completely remove the resource from the ResourceManager
         * \note The resource has to have a manual garbage collection mode in order to work
         * 
         * \param in_identifier Identifier of the resource to delete
         * \param in_loading_mode Resource loading mode. See ESynchronizationMode for more detailed information.
         * \return true if the resource has been successfully removed, false otherwise
         */
        RkBool UnloadResource(ResourceIdentifier const& in_identifier, ESynchronizationMode in_loading_mode = ESynchronizationMode::Asynchronous) noexcept;

        /**
         * \brief Returns the current number of resource operations being done. This number should go up in loading times, and stay close to 0 while playing.
         * \return Operation count
         */
        RkUint64 GetCurrentOperationCount() const noexcept;

        #pragma endregion

        #pragma region Operators

        ResourceManager& operator=(ResourceManager const& in_copy) = delete;
        ResourceManager& operator=(ResourceManager&&      in_move) = delete;

        #pragma endregion
};

#include "Resource/ResourceManager.inl"

END_RUKEN_NAMESPACE
