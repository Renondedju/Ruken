
#pragma once

#include <mutex>
#include <atomic>
#include <vector>

#include "Vulkan/Core/VulkanFence.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief High level interface managing fence allocation.
 *        Fences are cached to improve performance, but they are single use.
 *        The "Reset" function must be called before on this pool in order to reuse its fences.
 */
class FencePool
{
    private:

        #pragma region Members

        std::atomic<RkUint32>   m_index  {0u};
        std::vector<VulkanFence> m_fences {};

        mutable std::mutex m_mutex {};

        #pragma endregion

    public:

        #pragma region Constructors

        FencePool() = default;

        FencePool(FencePool const& in_copy) = delete;
        FencePool(FencePool&&      in_move) = delete;

        ~FencePool() = default;

        #pragma endregion

        #pragma region Methods

        /**
         * \return An available single use fence from the pool or a newly allocated one.
         * \note   This function is safe to call from any thread.
         */
        [[nodiscard]]
        VulkanFence& RequestFence() noexcept;

        /**
         * \brief 
         */
        [[nodiscard]]
        RkBool Wait() const noexcept;

        /**
         * \brief Resets all managed fences, meaning previously requested ones can be reused.
         * \note  This function must be called once at the beginning of a frame.
         */
        [[nodiscard]]
        RkBool Reset() noexcept;

        #pragma endregion

        #pragma region Operators

        FencePool& operator=(FencePool const& in_copy) = delete;
        FencePool& operator=(FencePool&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE