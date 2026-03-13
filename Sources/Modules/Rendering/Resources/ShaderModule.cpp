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
	descriptor_set_layout {[&]{
		auto const& device {in_service_provider.LocateService<RenderDevice>()->GetDevice()};
		constexpr std::array bindings {
			vk::DescriptorSetLayoutBinding {  // Per View Data
				.binding		    = 0,
				.descriptorType     = vk::DescriptorType::eUniformBuffer,
				.descriptorCount    = 1,
				.stageFlags			= vk::ShaderStageFlagBits::eVertex,
				.pImmutableSamplers = nullptr
			},
			vk::DescriptorSetLayoutBinding { // Per Instance Data
				.binding		    = 1,
				.descriptorType     = vk::DescriptorType::eStorageBuffer,
				.descriptorCount    = 1,
				.stageFlags			= vk::ShaderStageFlagBits::eVertex,
				.pImmutableSamplers = nullptr
			}
		};

		return vk::raii::DescriptorSetLayout {device, vk::DescriptorSetLayoutCreateInfo {
			.flags		  = {},
			.bindingCount = bindings.size(),
			.pBindings    = bindings.data()
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

        vk::PipelineDynamicStateCreateInfo dynamic_state {
            .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
            .pDynamicStates    = dynamicStates.data()
        };

        // --- Vertex input
		auto bindingDescription    = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

        vk::PipelineVertexInputStateCreateInfo vertex_input_info {
        	.vertexBindingDescriptionCount   = 1,
        	.pVertexBindingDescriptions      = &bindingDescription,
			.vertexAttributeDescriptionCount = attributeDescriptions.size(),
        	.pVertexAttributeDescriptions    = attributeDescriptions.data()
        };

        // --- Input assembly
        vk::PipelineInputAssemblyStateCreateInfo input_assembly {
            .topology               = vk::PrimitiveTopology::eTriangleList,
            .primitiveRestartEnable = vk::False
        };

        // --- Viewport and scissor
        vk::PipelineViewportStateCreateInfo viewport_state {
            .viewportCount = 1,
            .scissorCount  = 1,
        };

        // --- Rasterizer
        vk::PipelineRasterizationStateCreateInfo rasterizer {
            .depthClampEnable        = vk::False,
            .rasterizerDiscardEnable = vk::False,
            .polygonMode             = vk::PolygonMode::eFill,
            .cullMode                = vk::CullModeFlagBits::eBack,
            .frontFace               = vk::FrontFace::eClockwise,
            .depthBiasEnable         = vk::False,
            .lineWidth               = 1.0f
        };

		vk::PipelineDepthStencilStateCreateInfo depth_stencil {
			.depthTestEnable       = vk::True,
			.depthWriteEnable      = vk::True,
			.depthCompareOp        = vk::CompareOp::eLess,
			.depthBoundsTestEnable = vk::False,
			.stencilTestEnable     = vk::False
		};

        // --- Multisampling
        vk::PipelineMultisampleStateCreateInfo multisampling {
            .rasterizationSamples = vk::SampleCountFlagBits::e1,
            .sampleShadingEnable  = vk::False
        };

        // --- Color blending
        vk::PipelineColorBlendAttachmentState color_blend_attachment {
            .blendEnable    = vk::False,
            .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                              vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        };

        vk::PipelineColorBlendStateCreateInfo color_blending {
            .logicOpEnable   = vk::False,
            .attachmentCount = 1,
            .pAttachments    = &color_blend_attachment,
        };

        // --- Create the pipeline
		std::vector formats { vk::Format::eR8G8B8A8Srgb };
        vk::PipelineRenderingCreateInfo pipeline_rendering_create_info {
             .colorAttachmentCount    = static_cast<uint32_t>(formats.size()),
             .pColorAttachmentFormats = formats.data(),
        	.depthAttachmentFormat    = vk::Format::eD32Sfloat
        };

		return vk::raii::Pipeline(device, nullptr, vk::GraphicsPipelineCreateInfo {
			.pNext      		 = &pipeline_rendering_create_info,
			.stageCount 		 = static_cast<uint32_t>(stages.size()),
			.pStages    		 = stages.data(),
			.pVertexInputState   = &vertex_input_info,
			.pInputAssemblyState = &input_assembly,
			.pViewportState      = &viewport_state,
			.pRasterizationState = &rasterizer,
			.pMultisampleState   = &multisampling,
			.pDepthStencilState  = &depth_stencil,
			.pColorBlendState    = &color_blending,
			.pDynamicState       = &dynamic_state,
			.layout              = pipeline_layout,
			.renderPass          = nullptr,
			.subpass             = 0,
		});
	}()}
{}
