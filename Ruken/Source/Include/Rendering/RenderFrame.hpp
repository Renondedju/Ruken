#pragma once

#include "Maths/Matrix/Matrix.hpp"

#include "Rendering/RenderObjects/Buffer.hpp"
#include "Rendering/RenderObjects/TimelineSemaphore.hpp"

#include "Rendering/RenderObjectPools/CommandPool.hpp"
#include "Rendering/RenderObjectPools/SemaphorePool.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;
class RenderGraph;

struct CameraData
{
    Matrix<4, 4> view;
    Matrix<4, 4> proj;
};

class RenderFrame
{
    private:

        #pragma region Members

        Logger*       m_logger {nullptr};
        RenderDevice* m_device {nullptr};
        RenderGraph*  m_graph  {nullptr};

        TimelineSemaphore m_timeline_semaphore;
        SemaphorePool     m_semaphore_pool;
        CommandPool       m_graphics_command_pool;
        CommandPool       m_compute_command_pool;
        CommandPool       m_transfer_command_pool;
        Buffer            m_draw_storage_buffer;
        Buffer            m_transform_storage_buffer;
        Buffer            m_material_storage_buffer;
        Buffer            m_camera_uniform_buffer;

        vk::DescriptorPool m_descriptor_pool;
        vk::DescriptorSet  m_texture_descriptor_set;
        vk::DescriptorSet  m_frame_descriptor_set;
        vk::DescriptorSet  m_camera_descriptor_set;

        #pragma endregion

    public:

        #pragma region Constructors

        RenderFrame(Logger* in_logger, RenderDevice* in_device, RenderGraph* in_graph) noexcept;

        RenderFrame(RenderFrame const& in_copy) = delete;
        RenderFrame(RenderFrame&&      in_move) = delete;

        ~RenderFrame() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid Reset() noexcept;

        vk::DescriptorSet const& GetTextureDescriptorSet() const noexcept;
        vk::DescriptorSet const& GetFrameDescriptorSet  () const noexcept;
        vk::DescriptorSet const& GetCameraDescriptorSet () const noexcept;

        TimelineSemaphore& GetTimelineSemaphore     () noexcept;
        SemaphorePool&     GetSemaphorePool         () noexcept;
        CommandPool&       GetGraphicsCommandPool   () noexcept;
        CommandPool&       GetComputeCommandPool    () noexcept;
        CommandPool&       GetTransferCommandPool   () noexcept;
        Buffer&            GetDrawStorageBuffer     () noexcept;
        Buffer&            GetMaterialStorageBuffer () noexcept;
        Buffer&            GetTransformStorageBuffer() noexcept;
        Buffer&            GetCameraUniformBuffer   () noexcept;

        #pragma endregion

        #pragma region Operators

        RenderFrame& operator=(RenderFrame const& in_copy) = delete;
        RenderFrame& operator=(RenderFrame&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE
