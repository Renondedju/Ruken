
#pragma once

#include "Build/Namespace.hpp"
#include "Types/FundamentalTypes.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief Base resource interface. Allows the implementation of any type of resource.
 * \see ResourceManager class
 * \see BaseResourceLoadingDescriptor class
 */
class IResource
{
    public:

        #pragma region Constructors

        IResource()                            noexcept = default;
        IResource(IResource const& in_copy)    noexcept = default;
        IResource(IResource&& in_move)        noexcept = default;

        virtual ~IResource()                         = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Loads the resource
         * 
         * May be called from any thread.
         * This method might be called multiple times if a previous request failed.
         * 
         * \param in_manager Resource manager instance. This is useful to request dependencies or resolve assets name/path.
         * \param in_descriptor Resource loading descriptor. This structure can be inherited to pass custom parameters to the loader
         */
        virtual RkVoid Load(class ResourceManager& in_manager, class ResourceLoadingDescriptor const& in_descriptor) = 0;
        
        /**
         * \brief Reloads the resource
         * 
         * \note Loading the resource before unloading the previous one is a good idea in case of failure.
         * \note By doing so, the old resource will be untouched and still available
         * 
         * May be called from any thread.
         * This will be called only if load() has successfully be called before
         * 
         * \param in_manager Resource manager instance. This is useful to request dependencies or resolve assets name/path.
         */
        virtual RkVoid Reload(class ResourceManager& in_manager) = 0;
        
        /**
         * \brief Unloads the resource
         * 
         * May be called from any thread.
         * This will be called only if load() has successfully be called before
         * 
         * \param in_manager Resource manager instance. This is useful to request dependencies or resolve assets name/path.
         */
        virtual RkVoid Unload(class ResourceManager& in_manager) noexcept = 0;

        #pragma endregion

        #pragma region Operators

        IResource& operator=(IResource const&    in_copy) noexcept = default;
        IResource& operator=(IResource&&        in_move) noexcept = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE