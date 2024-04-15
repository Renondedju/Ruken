
#pragma once

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

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

        static std::vector<RkChar const*>                m_required_extensions;
        static std::vector<RkChar const*>                m_required_validation_layers;
        static std::vector<VkValidationFeatureEnableEXT>  m_enabled_validation_features;
        static std::vector<VkValidationFeatureDisableEXT> m_disabled_validation_features;

        VkInstance m_handle {nullptr};

        #pragma endregion

        #pragma region Methods

        static RkBool CheckInstanceExtensions() noexcept;
        static RkBool CheckValidationLayers  () noexcept;

        RkBool CreateInstance() noexcept;

        #pragma endregion

    public:

        #pragma region Constructors

        VulkanInstance() noexcept;

        VulkanInstance(VulkanInstance const& in_copy) = delete;
        VulkanInstance(VulkanInstance&&      in_move) = delete;

        ~VulkanInstance() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] static std::vector<RkChar const*>                const& GetRequiredExtensions        () noexcept;
        [[nodiscard]] static std::vector<RkChar const*>                const& GetRequiredValidationLayers  () noexcept;
        [[nodiscard]] static std::vector<VkValidationFeatureEnableEXT>  const& GetEnabledValidationFeatures () noexcept;
        [[nodiscard]] static std::vector<VkValidationFeatureDisableEXT> const& GetDisabledValidationFeatures() noexcept;

        [[nodiscard]] RkBool           IsValid  () const noexcept;
        [[nodiscard]] VkInstance const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanInstance& operator=(VulkanInstance const& in_copy) = delete;
        VulkanInstance& operator=(VulkanInstance&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE