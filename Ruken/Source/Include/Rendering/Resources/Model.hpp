#pragma once

#include "Maths/Vector/DistanceVector2.hpp"
#include "Maths/Vector/DistanceVector3.hpp"

#include "Maths/Matrix/Matrix.hpp"

#include "Rendering/UV.hpp"

#include "Resource/IResource.hpp"

#include "Rendering/RenderObjects/Buffer.hpp"

BEGIN_RUKEN_NAMESPACE

struct Vertex
{
    Vector3m position;
    Vector3m color;
    UV uv;

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

struct TransformData
{
    Matrix<4, 4> model;
};

struct DrawData
{
    RkUint32 material_index;
    RkUint32 transform_index;
};

class Renderer;

class Model final : public IResource
{
    private:

        #pragma region Members

        Renderer* m_renderer;

        vk::DeviceSize m_offset;
        vk::DeviceSize m_count;

        std::unique_ptr<Buffer> m_buffer;

        #pragma endregion

    public:

        #pragma region Constructors

        Model(Renderer* in_renderer, std::string_view in_path) noexcept;

        Model(Model const& in_copy) = delete;
        Model(Model&&      in_move) = delete;

        ~Model() noexcept override;

        #pragma endregion

        RkVoid Render(vk::CommandBuffer const& in_command_buffer);

        RkVoid Load  (ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor) override;
        RkVoid Reload(ResourceManager& in_manager)                                                 override;
        RkVoid Unload(ResourceManager& in_manager)                                                 noexcept override;

        #pragma region Operators

        Model& operator=(Model const& in_copy) = delete;
        Model& operator=(Model&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE