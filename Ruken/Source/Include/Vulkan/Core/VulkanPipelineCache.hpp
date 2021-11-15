
#pragma once

#include <vector>
#include <string_view>

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief RAII-class wrapping a 'VkPipelineCache' object.
 *        Pipeline cache objects allow the result of pipeline construction to be reused between pipelines and between runs of an application.
 *        Reuse between pipelines is achieved by passing the same pipeline cache object when creating multiple related pipelines.
 *        Reuse across runs of an application is achieved by retrieving pipeline cache contents in one run of an application,
 *        saving the contents, and using them to pre-initialize a pipeline cache on a subsequent run.
 */
class VulkanPipelineCache
{
    private:

        #pragma region Members

        VkPipelineCache m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        explicit VulkanPipelineCache(std::string_view in_path);

        VulkanPipelineCache(VulkanPipelineCache const& in_copy) = delete;
        VulkanPipelineCache(VulkanPipelineCache&&      in_move) = delete;

        ~VulkanPipelineCache() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \brief Saves the data retrieved from this pipeline cache to the specified file for reuse.
         */
        RkVoid SaveData(std::string_view in_path) const;

        [[nodiscard]] VkPipeline CreateGraphicsPipeline(VkGraphicsPipelineCreateInfo const& in_pipeline_create_info) const noexcept;
        [[nodiscard]] VkPipeline CreateComputePipeline (VkComputePipelineCreateInfo  const& in_pipeline_create_info) const noexcept;

        [[nodiscard]] std::vector<VkPipeline> CreateGraphicsPipelines(std::vector<VkGraphicsPipelineCreateInfo> const& in_pipeline_create_infos) const noexcept;
        [[nodiscard]] std::vector<VkPipeline> CreateComputePipelines (std::vector<VkComputePipelineCreateInfo>  const& in_pipeline_create_infos) const noexcept;

        [[nodiscard]]
        VkPipelineCache const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanPipelineCache& operator=(VulkanPipelineCache const& in_copy) = delete;
        VulkanPipelineCache& operator=(VulkanPipelineCache&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE