
#include "Vulkan/Core/VulkanShaderModule.hpp"

#include "Vulkan/Utilities/VulkanDebug.hpp"
#include "Vulkan/Utilities/VulkanLoader.hpp"

USING_RUKEN_NAMESPACE

#pragma region Constructors

VulkanShaderModule::VulkanShaderModule(std::vector<RkUint32> const& in_code) noexcept
{
    VkShaderModuleCreateInfo shader_module_create_info = {};

    shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.codeSize = in_code.size() * sizeof(RkUint32);
    shader_module_create_info.pCode    = in_code.data();

    VK_CHECK(vkCreateShaderModule(VulkanLoader::GetLoadedDevice(), &shader_module_create_info, nullptr, &m_handle));
}

VulkanShaderModule::VulkanShaderModule(VulkanShaderModule&& in_move) noexcept:
    m_handle {in_move.m_handle}
{
    in_move.m_handle = nullptr;
}

VulkanShaderModule::~VulkanShaderModule() noexcept
{
    if (!m_handle)
        return;

    vkDestroyShaderModule(VulkanLoader::GetLoadedDevice(), m_handle, nullptr);
}

#pragma endregion

#pragma region Methods

RkBool VulkanShaderModule::IsValid() const noexcept
{
    return m_handle != nullptr;
}

VkShaderModule const& VulkanShaderModule::GetHandle() const noexcept
{
    return m_handle;
}

#pragma endregion

#pragma region Operators

VulkanShaderModule& VulkanShaderModule::operator=(VulkanShaderModule&& in_move) noexcept
{
    m_handle = in_move.m_handle;

    in_move.m_handle = nullptr;

    return *this;
}

#pragma endregion