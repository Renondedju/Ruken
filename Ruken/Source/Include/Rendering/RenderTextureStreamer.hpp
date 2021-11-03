#pragma once

#include "Rendering/Resources/Texture.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;

class RenderTextureStreamer
{
    private:

        Logger*       m_logger;
        RenderDevice* m_device;

        RkUint32 m_index;

        vk::DescriptorSetLayout m_descriptor_set_layout;
        vk::DescriptorPool      m_descriptor_pool;
        vk::DescriptorSet       m_descriptor_set;

    public:

        #pragma region Operators

        RenderTextureStreamer(Logger* in_logger, RenderDevice* in_device) noexcept;

        RenderTextureStreamer(RenderTextureStreamer const& in_copy) = delete;
        RenderTextureStreamer(RenderTextureStreamer&&      in_move) = delete;

        ~RenderTextureStreamer() noexcept;

        #pragma endregion

        #pragma region Methods

        RkUint32 RegisterTexture(Texture const& in_texture) noexcept;

        vk::DescriptorSetLayout const& GetDescriptorSetLayout() const noexcept;
        vk::DescriptorSet       const& GetDescriptorSet      () const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderTextureStreamer& operator=(RenderTextureStreamer const& in_copy) = delete;
        RenderTextureStreamer& operator=(RenderTextureStreamer&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE