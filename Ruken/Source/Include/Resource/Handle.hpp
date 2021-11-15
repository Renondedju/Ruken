
#pragma once

#include "Utility/Todo.hpp"

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

#include "Resource/IResource.hpp"
#include "Resource/ResourceManifest.hpp"
#include "Resource/Enums/EResourceStatus.hpp"

#include <type_traits>

BEGIN_RUKEN_NAMESPACE

/**
 * \brief A handle is a smart pointer to a resource managed by the resource manager.
 * 
 * This class allows for quick and simple access to resources. May be used on any thread.
 *    
 * \tparam TResource_Type Type of resource kept by the manager 
 */
template <typename TResource_Type>
class Handle
{
    static_assert(std::is_base_of_v<IResource, TResource_Type>, "Handles can only be used on classes that implements the IResource interface");

    friend class ResourceManager;

    private:

        #pragma region Variables
        
        ResourceManifest* m_manifest;

        #pragma endregion

    public:

        using Type = TResource_Type;

        #pragma region Constructors

        Handle() = default;
        Handle(Handle const& in_copy) noexcept;
        Handle(Handle&&      in_move) noexcept;
        ~Handle();

        // Creation from a resource manifest, resource manager exclusive
        explicit Handle(ResourceManifest* in_manifest);

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Returns the resource.
         * Returns nullptr instead if the resource isn't available yet or has been invalidated.
         * 
         * \return Resource
         */
        [[nodiscard]] TResource_Type*       Get()       noexcept;
        [[nodiscard]] TResource_Type const* Get() const noexcept;

        /**
         * \brief Returns the current status of the resource
         * \return Resource status
         */
        [[nodiscard]]
        EResourceStatus Status() const noexcept;

        /**
         * \brief Shortcut for this->status() == EResourceStatus::Loaded
         * \return True if the resource is available and valid.
         */
        [[nodiscard]]
        RkBool Available() const noexcept;

        /**
         * \brief Checks if the resource is valid, **a valid resource isn't always available**
         * \return True if the resource is valid.
         */
        [[nodiscard]]
        RkBool Valid() const noexcept;

        /**
         * \brief Returns the number of references made to the resource
         * \return reference count
         */
        [[nodiscard]]
        ResourceManifest::ReferenceCountType ReferenceCount() const noexcept;

        /**
         * \brief Waits until the resource becomes available
         * \note If the underlying resource manager hasn't been set, this method won't have any effects
         * \return True if the resource is valid, false otherwise.
         */
        RkBool WaitForValidity(RkFloat in_timeout) const noexcept;

        /**
         * \brief Returns the garbage collection strategy (get)
         * \return GC strategy
         */
        [[nodiscard]]
        EResourceGCStrategy const& GCStrategy() const noexcept;

        /**
         * \brief Sets the garbage collection strategy
         * \param in_gc_strategy New GC strategy
         * \return GC strategy
         */
        EResourceGCStrategy GCStrategy(EResourceGCStrategy in_gc_strategy) const noexcept;

        #pragma endregion

        #pragma region Constructors

        Handle& operator=(ResourceManifest* in_manifest) noexcept;
        Handle& operator=(Handle const&     in_copy)     noexcept;
        Handle& operator=(Handle&&          in_move)     noexcept;

        /**
         * \brief Downcast support for handles
         * \tparam TDerived Derived resource class to cast to
         */
        template<typename TDerived>
        explicit operator Handle<TDerived>() const;

        #pragma endregion
};

#include "Resource/Handle.inl"

END_RUKEN_NAMESPACE