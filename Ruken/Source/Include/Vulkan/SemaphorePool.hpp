
#pragma once

#include <mutex>
#include <atomic>
#include <vector>

#include "Vulkan/Core/VulkanSemaphore.hpp"
#include "Vulkan/Core/VulkanTimelineSemaphore.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief High level interface managing binary and timeline semaphore allocation.
 *        Semaphores are cached to improve performance, but they are single use.
 *        The "Reset" function must be called before on this pool in order to reuse its semaphores.
 */
class SemaphorePool
{
    private:

        #pragma region Members

        std::atomic<RkUint32> m_semaphore_index          {0u};
        std::atomic<RkUint32> m_timeline_semaphore_index {0u};

        std::vector<VulkanSemaphore>         m_semaphores          {};
        std::vector<VulkanTimelineSemaphore> m_timeline_semaphores {};

        mutable std::mutex m_mutex {};

        #pragma endregion

    public:

        #pragma region Constructors

        SemaphorePool() = default;

        SemaphorePool(SemaphorePool const& in_copy) = delete;
        SemaphorePool(SemaphorePool&&      in_move) = delete;

        ~SemaphorePool() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \return An available single use binary semaphore of the pool or a newly allocated one.
         * \note   This function is safe to call from any thread.
         */
        [[nodiscard]]
        VulkanSemaphore& RequestSemaphore() noexcept;

        /**
         * \return An available single use timeline semaphore of the pool or a newly allocated one.
         * \note   This function is safe to call from any thread.
         */
        [[nodiscard]]
        VulkanTimelineSemaphore& RequestTimelineSemaphore() noexcept;

        /**
         * \brief Resets all managed semaphores, meaning previously requested ones can be reused.
         * \note  This function must be called once at the beginning of a frame.
         */
        [[nodiscard]]
        RkBool Reset() noexcept;

        #pragma endregion

        #pragma region Operators

        SemaphorePool& operator=(SemaphorePool const& in_copy) = delete;
        SemaphorePool& operator=(SemaphorePool&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE