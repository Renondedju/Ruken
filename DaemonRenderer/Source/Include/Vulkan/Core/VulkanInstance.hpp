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

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_DAEMON_NAMESPACE

/**
 * \brief RAII-class wrapping a 'VkInstance' object.
 *        There is no global state in Vulkan and all per-application state is stored in a 'VkInstance' object.
 *        Creating a 'VkInstance' object initializes the Vulkan library and allows the application to
 *        pass information about itself to the implementation.
 */
class VulkanInstance
{
    private:

        #pragma region Members

        static std::vector<DAEchar const*>                m_required_extensions;
        static std::vector<DAEchar const*>                m_required_validation_layers;
        static std::vector<VkValidationFeatureEnableEXT>  m_enabled_validation_features;
        static std::vector<VkValidationFeatureDisableEXT> m_disabled_validation_features;

        VkInstance m_handle {nullptr};

        #pragma endregion

        #pragma region Methods

        static DAEbool CheckInstanceExtensions() noexcept;
        static DAEbool CheckValidationLayers  () noexcept;

        DAEbool CreateInstance() noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        VulkanInstance() noexcept;

        VulkanInstance(VulkanInstance const& in_copy) = delete;
        VulkanInstance(VulkanInstance&&      in_move) = delete;

        ~VulkanInstance() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] static std::vector<DAEchar const*>                const& GetRequiredExtensions        () noexcept;
        [[nodiscard]] static std::vector<DAEchar const*>                const& GetRequiredValidationLayers  () noexcept;
        [[nodiscard]] static std::vector<VkValidationFeatureEnableEXT>  const& GetEnabledValidationFeatures () noexcept;
        [[nodiscard]] static std::vector<VkValidationFeatureDisableEXT> const& GetDisabledValidationFeatures() noexcept;

        [[nodiscard]] DAEbool           IsValid  () const noexcept;
        [[nodiscard]] VkInstance const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanInstance& operator=(VulkanInstance const& in_copy) = delete;
        VulkanInstance& operator=(VulkanInstance&&      in_move) = delete;

        #pragma endregion
};

END_DAEMON_NAMESPACE