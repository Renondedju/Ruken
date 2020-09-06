/*
 *  MIT License
 *
 *  Copyright (c) 2019-2020 Basile Combet, Philippe Yi
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

#pragma once

#include "Config.hpp"
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