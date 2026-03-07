#include "Rendering/Resources/ShaderModule.hpp"
#include "Rendering/Vertex.hpp"

USING_RUKEN_NAMESPACE

ShaderModule::ShaderModule(ServiceProvider     const& in_service_provider,
						   std::vector<RkByte> const& in_spirv_code) noexcept:
	module     {in_service_provider.LocateService<RenderDevice>()->GetDevice(), vk::ShaderModuleCreateInfo {
		.codeSize = in_spirv_code.size() * sizeof(RkByte),
		.pCode    = reinterpret_cast<const uint32_t*>(in_spirv_code.data())
	}},
	stages {
		{
			.stage  = vk::ShaderStageFlagBits::eFragment,
			.module = module,
			.pName  = "fragMain",
			.pSpecializationInfo = nullptr
		}, {
			.stage  = vk::ShaderStageFlagBits::eVertex,
			.module = module,
			.pName  = "vertMain",
			.pSpecializationInfo = nullptr
		}
	},
	descriptor_set_layout {[&](){
		auto const& device {in_service_provider.LocateService<RenderDevice>()->GetDevice()};
		constexpr vk::DescriptorSetLayoutBinding ubo_layout_binding {
			.binding		    = 0,
			.descriptorType     = vk::DescriptorType::eUniformBuffer,
			.descriptorCount    = 1,
			.stageFlags			= vk::ShaderStageFlagBits::eVertex,
			.pImmutableSamplers = nullptr
		};

		return vk::raii::DescriptorSetLayout {device, vk::DescriptorSetLayoutCreateInfo {
			.flags		  = {},
			.bindingCount = 1,
			.pBindings    = &ubo_layout_binding
		}};
	}()},
	pipeline_layout {in_service_provider.LocateService<RenderDevice>()->GetDevice(), vk::PipelineLayoutCreateInfo {
		.setLayoutCount         = 1,
		.pSetLayouts			= &*descriptor_set_layout,
		.pushConstantRangeCount = 0,
	}},
	pipeline {[&] {
		vk::raii::Device const& device	{in_service_provider.LocateService<RenderDevice>()->GetDevice()};
		std::vector		  dynamicStates {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicState {
            .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
            .pDynamicStates    = dynamicStates.data()
        };

        // --- Vertex input
		auto bindingDescription    = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo {
        	.vertexBindingDescriptionCount   = 1,
        	.pVertexBindingDescriptions      = &bindingDescription,
			.vertexAttributeDescriptionCount = attributeDescriptions.size(),
        	.pVertexAttributeDescriptions    = attributeDescriptions.data()
        };

        // --- Input assembly
        vk::PipelineInputAssemblyStateCreateInfo inputAssembly {
            .topology               = vk::PrimitiveTopology::eTriangleList,
            .primitiveRestartEnable = VK_FALSE
        };

        // --- Viewport and scissor
        vk::PipelineViewportStateCreateInfo viewportState {
            .viewportCount = 1,
            .scissorCount  = 1,
        };

        // --- Rasterizer
        vk::PipelineRasterizationStateCreateInfo rasterizer {
            .depthClampEnable        = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode             = vk::PolygonMode::eFill,
            .cullMode                = vk::CullModeFlagBits::eBack,
            .frontFace               = vk::FrontFace::eClockwise,
            .depthBiasEnable         = VK_FALSE,
            .lineWidth               = 1.0f
        };

        // --- Multisampling
        vk::PipelineMultisampleStateCreateInfo multisampling {
            .rasterizationSamples = vk::SampleCountFlagBits::e1,
            .sampleShadingEnable  = VK_FALSE
        };

        // --- Color blending
        vk::PipelineColorBlendAttachmentState colorBlendAttachment {
            .blendEnable    = VK_FALSE,
            .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                              vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        };

        vk::PipelineColorBlendStateCreateInfo colorBlending {
            .logicOpEnable   = VK_FALSE,
            .attachmentCount = 1,
            .pAttachments    = &colorBlendAttachment,
        };

        // --- Create the pipeline
		std::vector formats { vk::Format::eR8G8B8A8Srgb };
        vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo {
             .colorAttachmentCount    = static_cast<uint32_t>(formats.size()),
             .pColorAttachmentFormats = formats.data()
        };

		return vk::raii::Pipeline(device, nullptr, vk::GraphicsPipelineCreateInfo {
			.pNext      		 = &pipelineRenderingCreateInfo,
			.stageCount 		 = static_cast<uint32_t>(stages.size()),
			.pStages    		 = stages.data(),
			.pVertexInputState   = &vertexInputInfo,
			.pInputAssemblyState = &inputAssembly,
			.pViewportState      = &viewportState,
			.pRasterizationState = &rasterizer,
			.pMultisampleState   = &multisampling,
			.pColorBlendState    = &colorBlending,
			.pDynamicState       = &dynamicState,
			.layout              = pipeline_layout,
			.renderPass          = nullptr,
			.subpass             = 0,
		});
	}()}
{}
