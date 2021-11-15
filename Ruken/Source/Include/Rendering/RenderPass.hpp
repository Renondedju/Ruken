#pragma once

#include <unordered_set>

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;
class RenderGraph;

class RenderResource
{
    public:

        std::string name;

        std::unordered_set<RkUint32> read_in_passes;
        std::unordered_set<RkUint32> written_in_passes;
};

struct ImageInfo
{
    vk::Format          format;
    vk::Extent3D        extent;
    vk::ImageUsageFlags usage;
};

class RenderImageResource : public RenderResource
{
    public:

        ImageInfo info;
};

struct BufferInfo
{
    vk::DeviceSize       size;
    vk::BufferUsageFlags usage;
};

class RenderBufferResource : public RenderResource
{
    public:

        BufferInfo info;
};

class RenderPass
{
    private:

        Logger*       m_logger;
        RenderDevice* m_device;
        RenderGraph*  m_graph;

        vk::RenderPass  m_handle;
        vk::Framebuffer m_framebuffer;

        RenderImageResource* m_depth_stencil_input;
        RenderImageResource* m_depth_stencil_output;

        std::vector<RenderImageResource*> m_color_inputs;
        std::vector<RenderImageResource*> m_color_outputs;

        std::function<RkVoid(vk::CommandBuffer const&)> m_callback;

        #pragma region Methods

        

        #pragma endregion

    public:

        #pragma region Constructors

        RenderPass(Logger* in_logger, RenderDevice* in_device, RenderGraph* in_graph) noexcept;

        RenderPass(RenderPass const& in_copy) = delete;
        RenderPass(RenderPass&&      in_move) = delete;

        ~RenderPass() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid Build  () noexcept;
        RkVoid Execute() const noexcept;

        RkVoid AddColorInput (std::string const& in_name);
        RkVoid AddColorOutput(std::string const& in_name, ImageInfo const& in_image_info);

        RkVoid SetDepthStencilInput (std::string const& in_name);
        RkVoid SetDepthStencilOutput(std::string const& in_name, ImageInfo const& in_image_info);

        RkVoid SetCallback(std::function<RkVoid(vk::CommandBuffer const&)>&& in_callback) noexcept;

        vk::RenderPass const& GetHandle() const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderPass& operator=(RenderPass const& in_copy) = delete;
        RenderPass& operator=(RenderPass&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE