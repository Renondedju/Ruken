
#pragma once

#include <atomic>

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"
#include "Resource/Enums/EResourceStatus.hpp"
#include "Resource/Enums/EResourceGCStrategy.hpp"
#include "Resource/ResourceIdentifier.hpp"

#ifndef RUKEN_RESOURCE_MANIFEST_STORE_IDENTIFIER
    #define RUKEN_RESOURCE_MANIFEST_DONT_STORE_IDENTIFIER
#endif

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A resource manifest is a class used by the resource manager to store the resource as well as it's corresponding data
 * \note  A ResourceManifest cannot be moved or copied and must be unique.
 * 
 * Everything contained in this class is susceptible to be accessed from multiple threads at once
 * 
 * The resource manifest is only a container to describe some data.
 * So any data stored in this struct must be manually deleted before deleting the struct itself.
 * 
 * Every operation done on this object is driven by the resource manager.
 */
struct ResourceManifest
{
    private:

        #pragma region Members

#ifdef RUKEN_RESOURCE_MANIFEST_STORE_IDENTIFIER
        // This is only used for debug messages
        const ResourceIdentifier m_identifier;
#endif

        #pragma endregion

    public:

        typedef RkUint32 ReferenceCountType;

        #pragma region Members

        // Pointer to the resource itself
        std::atomic<class IResource*> data;

        // Number of references to the resource
        std::atomic<ReferenceCountType> reference_count;

        // Garbage collection (GC) strategy of the resource
        std::atomic<EResourceGCStrategy> gc_strategy;

        // Status / availability of the resource
        std::atomic<EResourceStatus> status;

           #pragma endregion

        #pragma region Constructors

        ResourceManifest() noexcept;

        ResourceManifest(ResourceIdentifier const& in_identifier, class IResource* in_data, EResourceGCStrategy in_gc_strategy) noexcept; 

        ResourceManifest(ResourceManifest const& in_copy) noexcept = delete;
        ResourceManifest(ResourceManifest&&         in_move) noexcept = delete;
        ~ResourceManifest()                                           = default;

        #pragma endregion

        #pragma region Methods 

        /**
         * \brief Returns the identifier of the manifest, if RUKEN_RESOURCE_MANIFEST_STORE_IDENTIFIER isn't defined, this will return a default identifier instead
         * \return Resource identifier
         */
#ifdef RUKEN_RESOURCE_MANIFEST_STORE_IDENTIFIER
        [[nodiscard]] ResourceIdentifier const& GetIdentifier() const noexcept;
#else
        [[nodiscard]] ResourceIdentifier        GetIdentifier() const noexcept;
#endif

        #pragma endregion 

        #pragma region Operators

        ResourceManifest& operator=(ResourceManifest const& in_copy) noexcept = delete;
        ResourceManifest& operator=(ResourceManifest&& in_move)      noexcept = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE