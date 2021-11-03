#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "Rendering/RenderTarget.hpp"

#include "Rendering/RenderObjects/Buffer.hpp"
#include "Rendering/RenderObjects/TimelineSemaphore.hpp"

#include "Rendering/RenderObjectPools/CommandPool.hpp"
#include "Rendering/RenderObjectPools/SemaphorePool.hpp"
#include "Rendering/RenderObjectPools/RenderTargetPool.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class Renderer;

struct CameraData
{
    glm::mat4 view;
    glm::mat4 proj;
};

class RenderFrame
{
    private:

        #pragma region Members

        Logger*   m_logger {nullptr};
        Renderer* m_renderer {nullptr};

        RkUint32          m_index;
        TimelineSemaphore m_timeline_semaphore;
        SemaphorePool     m_semaphore_pool;
        CommandPool       m_graphics_command_pool;
        CommandPool       m_compute_command_pool;
        CommandPool       m_transfer_command_pool;
        RenderTargetPool  m_render_target_pool;
        Buffer            m_draw_storage_buffer;
        Buffer            m_transform_storage_buffer;
        Buffer            m_material_storage_buffer;
        Buffer            m_camera_uniform_buffer;

        vk::DescriptorPool m_descriptor_pool;
        vk::DescriptorSet  m_frame_descriptor_set;
        vk::DescriptorSet  m_camera_descriptor_set;

        #pragma endregion

    public:

        #pragma region Constructors

        RenderFrame(Logger* in_logger, Renderer* in_renderer, RkUint32 in_index) noexcept;

        RenderFrame(RenderFrame const& in_copy) = delete;
        RenderFrame(RenderFrame&&      in_move) = delete;

        ~RenderFrame() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid Reset() noexcept;

        vk::DescriptorSet const& GetFrameDescriptorSet () const noexcept;
        vk::DescriptorSet const& GetCameraDescriptorSet() const noexcept;

        RkUint32           GetIndex                 () const noexcept;
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
