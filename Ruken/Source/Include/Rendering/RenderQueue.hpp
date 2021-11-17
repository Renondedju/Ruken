#pragma once

#include <mutex>
#include <unordered_map>

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class RenderDevice;

class RenderQueue
{
    private:

        #pragma region Members

        std::mutex m_mutex;

        Logger*       m_logger {nullptr};
        RenderDevice* m_device {nullptr};

        RkUint32 m_family_index {UINT32_MAX};

        vk::Queue m_queue {};

        std::unordered_map<std::thread::id, vk::CommandPool> m_command_pools {};
        std::unordered_map<std::thread::id, vk::Fence>       m_fences        {};

        #pragma endregion

    public:

        #pragma region Constructors

        RenderQueue(Logger* in_logger, RenderDevice* in_device, RkUint32 in_family_index, RkUint32 in_queue_index) noexcept;

        RenderQueue(RenderQueue const& in_copy) = delete;
        RenderQueue(RenderQueue&&      in_move) = delete;

        ~RenderQueue() noexcept;

        #pragma endregion

        #pragma region Methods

        RkVoid            Submit                     (vk::SubmitInfo2KHR const& in_submit_info)    noexcept;
        RkVoid            Present                    (vk::PresentInfoKHR const& in_present_info)   noexcept;
        vk::CommandBuffer BeginSingleUseCommandBuffer()                                            noexcept;
        RkVoid            EndSingleUseCommandBuffer  (vk::CommandBuffer  const& in_command_buffer) const noexcept;

        #pragma endregion

        #pragma region Operators

        RenderQueue& operator=(RenderQueue const& in_copy) = delete;
        RenderQueue& operator=(RenderQueue&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE