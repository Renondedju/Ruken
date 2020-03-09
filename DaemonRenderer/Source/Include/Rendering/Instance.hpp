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

#pragma once

#include <Vulkan/vulkan.h>

#include "Types/FundamentalTypes.hpp"

#include "Containers/Vector.hpp"

BEGIN_DAEMON_NAMESPACE

class Renderer;

/**
 * \brief This class wraps a VkInstance object.
 *
 * \note There is no global state in Vulkan and all per-application state is stored in a VkInstance object.
 *       Creating a VkInstance object initializes the Vulkan library and allows the application to pass information about itself to the implementation.
 */
class Instance
{
    private:

        #pragma region Members

        VkInstance m_handle;

        Vector<DAEchar const*> m_required_extensions;
        Vector<DAEchar const*> m_required_layers;

        #pragma endregion

        #pragma region Methods

        /**
         * \return True if the required extensions are supported, else False.
         */
        [[nodiscard]] DAEbool CheckInstanceExtensions() const noexcept;

        /**
         * \return True if the required validation layers are supported, else False.
         */
        [[nodiscard]] DAEbool CheckValidationLayers() const noexcept;

        /**
         * \return True if a new Vulkan instance could be created, else False.
         */
        [[nodiscard]] DAEbool SetupInstance() noexcept;

        #pragma endregion

    public:

        #pragma region Constructors and Destructor

        Instance();

        Instance(Instance const&    in_copy) = delete;
        Instance(Instance&&         in_move) = delete;

        ~Instance() noexcept;

        #pragma endregion

        #pragma region Operators

        Instance& operator=(Instance const& in_copy) = delete;
        Instance& operator=(Instance&&      in_move) = delete;

        #pragma endregion

        #pragma region Methods

        /**
         * \param in_layer_name Name of the layer to retrieve extensions from.
         *
         * \return A list of the supported global extension properties.
         *
         * \note When 'in_layer_name' parameter is NULL, only extensions provided by the Vulkan implementation or by implicitly enabled layers are returned.
         *       When 'in_layer_name' is the name of a layer, the instance extensions provided by that layer are returned.
         */
        static Vector<VkExtensionProperties> GetSupportedExtensions(DAEchar const* in_layer_name = nullptr) noexcept;

        /**
         * \return A list of the supported global layer properties.
         */
        static Vector<VkLayerProperties> GetSupportedLayers() noexcept;

        /**
         * \return The opaque handle to the instance object.
         */
        [[nodiscard]] VkInstance GetHandle() const noexcept;

        /**
         * \return The list of the required instance extensions.
         */
        [[nodiscard]] Vector<DAEchar const*> const& GetRequiredExtensions() const noexcept;

        /**
         * \return The list of the required validation layers.
         */
        [[nodiscard]] Vector<DAEchar const*> const& GetRequiredLayers() const noexcept;

        #pragma endregion
};

END_DAEMON_NAMESPACE