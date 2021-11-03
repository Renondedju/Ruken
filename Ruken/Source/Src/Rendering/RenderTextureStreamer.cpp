#include "Rendering/RenderTextureStreamer.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

RenderTextureStreamer::RenderTextureStreamer(Logger* in_logger, RenderDevice* in_device) noexcept:
    m_logger {in_logger},
    m_device {in_device}
{
    vk::DescriptorSetLayoutBinding descriptor_set_layout_binding = {
        .binding         = 0U,
        .descriptorType  = vk::DescriptorType::eCombinedImageSampler,
        .descriptorCount = 1000U,
        .stageFlags      = vk::ShaderStageFlagBits::eFragment
    };

    vk::DescriptorBindingFlags descriptor_binding_flags = vk::DescriptorBindingFlagBits::eUpdateAfterBind
                                                        | vk::DescriptorBindingFlagBits::eUpdateUnusedWhilePending
                                                        | vk::DescriptorBindingFlagBits::ePartiallyBound
                                                        | vk::DescriptorBindingFlagBits::eVariableDescriptorCount;

    vk::DescriptorSetLayoutBindingFlagsCreateInfo descriptor_set_layout_binding_flags_create_info = {
        .bindingCount  = 1U,
        .pBindingFlags = &descriptor_binding_flags
    };

    vk::DescriptorSetLayoutCreateInfo descriptor_set_layout_create_info = {
        .pNext        = &descriptor_set_layout_binding_flags_create_info,
        .flags        = vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
        .bindingCount = 1U,
        .pBindings    = &descriptor_set_layout_binding
    };

    m_descriptor_set_layout = m_device->GetLogicalDevice().createDescriptorSetLayout(descriptor_set_layout_create_info).value;

    vk::DescriptorPoolSize descriptor_pool_size = {
        .type            = vk::DescriptorType::eCombinedImageSampler,
        .descriptorCount = descriptor_set_layout_binding.descriptorCount
    };

    vk::DescriptorPoolCreateInfo descriptor_pool_create_info = {
        .flags         = vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind,
        .maxSets       = 1U,
        .poolSizeCount = 1U,
        .pPoolSizes    = &descriptor_pool_size
    };

    m_descriptor_pool = m_device->GetLogicalDevice().createDescriptorPool(descriptor_pool_create_info).value;

    vk::DescriptorSetVariableDescriptorCountAllocateInfo descriptor_set_variable_descriptor_count_allocate_info = {
        .descriptorSetCount = 1U,
        .pDescriptorCounts  = &descriptor_set_layout_binding.descriptorCount
    };

    vk::DescriptorSetAllocateInfo descriptor_set_allocate_info = {
        .pNext              = &descriptor_set_variable_descriptor_count_allocate_info,
        .descriptorPool     = m_descriptor_pool,
        .descriptorSetCount = 1U,
        .pSetLayouts        = &m_descriptor_set_layout
    };

    m_descriptor_set = m_device->GetLogicalDevice().allocateDescriptorSets(descriptor_set_allocate_info).value.front();
}

RenderTextureStreamer::~RenderTextureStreamer() noexcept
{
    m_device->GetLogicalDevice().destroy(m_descriptor_set_layout);
    m_device->GetLogicalDevice().destroy(m_descriptor_pool);
}

RkUint32 RenderTextureStreamer::RegisterTexture(Texture const& in_texture) noexcept
{
    vk::DescriptorImageInfo texture_descriptor_image_info = {
        .sampler     = in_texture.GetImageSampler(),
        .imageView   = in_texture.GetImageView(),
        .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal
    };

    vk::WriteDescriptorSet write_descriptor = {
        .dstSet = m_descriptor_set,
        .dstBinding = 0,
        .dstArrayElement = m_index,
        .descriptorCount = 1,
        .descriptorType = vk::DescriptorType::eCombinedImageSampler,
        .pImageInfo = &texture_descriptor_image_info
    };

    m_device->GetLogicalDevice().updateDescriptorSets(write_descriptor, VK_NULL_HANDLE);

    return m_index++;
}

vk::DescriptorSetLayout const& RenderTextureStreamer::GetDescriptorSetLayout() const noexcept
{
    return m_descriptor_set_layout;
}

vk::DescriptorSet const& RenderTextureStreamer::GetDescriptorSet() const noexcept
{
    return m_descriptor_set;
}