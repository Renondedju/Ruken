#pragma once

#include "Rendering/Resources/DeviceObjectBase.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm/glm.hpp>

#include <glm/glm/gtc/matrix_transform.hpp>

BEGIN_RUKEN_NAMESPACE

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 uv;

    static vk::VertexInputBindingDescription get_binding_description()
    {
        return {
            .binding = 0,
            .stride = sizeof(Vertex),
            .inputRate = vk::VertexInputRate::eVertex
        };
    }

    static std::array<vk::VertexInputAttributeDescription, 3> get_attribute_descriptions()
    {
        return {
            {
                {
                    .location = 0,
                    .binding = 0,
                    .format = vk::Format::eR32G32B32Sfloat,
                    .offset = offsetof(Vertex, position)
                },
                {
                    .location = 1,
                    .binding = 0,
                    .format = vk::Format::eR32G32B32Sfloat,
                    .offset = offsetof(Vertex, color)
                },
                {
                    .location = 2,
                    .binding = 0,
                    .format = vk::Format::eR32G32Sfloat,
                    .offset = offsetof(Vertex, uv)
                }
            }
        };
    }
};

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

class Model : public DeviceObjectBase
{
    private:

        #pragma region Members

        vk::DeviceSize m_offset;
        vk::DeviceSize m_count;

        vk::Buffer         m_buffer;
        vk::Allocation     m_allocation;
        vk::AllocationInfo m_allocation_info;
        vk::Buffer         m_uniform_buffer;
        vk::Allocation     m_uniform_allocation;
        vk::DescriptorPool m_descriptor_pool;
        vk::DescriptorSet  m_descriptor_set;

        #pragma endregion

    public:

        #pragma region Constructors

        Model(RenderDevice* in_device, std::string_view in_path) noexcept;

        Model(Model const& in_copy) = default;
        Model(Model&&      in_move) = default;

        ~Model() noexcept override;

        #pragma endregion

        RkVoid Render(vk::CommandBuffer const& in_command_buffer);

        #pragma region Operators

        Model& operator=(Model const& in_copy) = default;
        Model& operator=(Model&&      in_move) = default;

        #pragma endregion
};

END_RUKEN_NAMESPACE