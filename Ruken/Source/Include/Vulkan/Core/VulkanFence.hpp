
#pragma once

#include "Vulkan/Utilities/VulkanConfig.hpp"

BEGIN_RUKEN_NAMESPACE

/**
 * \brief RAII-class wrapping a 'VkFence' object.
 *        Fences are a synchronization primitive that can be used to insert a dependency from a queue to the host.
 *        Fences have two states - signaled and unsignaled.
 *        A fence can be signaled as part of the execution of a queue submission command.
 *        Fences can be unsignaled on the host with "Reset".
 *        Fences can be waited on by the host with the "Wait" command, and the current state can be queried with "IsSignaled".
 */
class VulkanFence
{
    private:

        #pragma region Members

        VkFence m_handle {nullptr};

        #pragma endregion

    public:

        #pragma region Constructors

        VulkanFence() noexcept;

        VulkanFence(VulkanFence const& in_copy) = delete;
        VulkanFence(VulkanFence&&      in_move) noexcept;

        ~VulkanFence() noexcept;

        #pragma endregion

        #pragma region Methods

        /**
         * \return True if this fence could be reset to the unsignaled state, else False.
         */
        [[nodiscard]]
        RkBool Reset() const noexcept;

        /**
         * \brief If the condition is satisfied when this function is called, then it returns immediately.
         *        If the condition is not satisfied at the time this function is called,
         *        then this function will block and wait for the condition to become satisfied.
         */
        RkVoid Wait() const noexcept;

        [[nodiscard]] RkBool        IsSignaled() const noexcept;
        [[nodiscard]] VkFence const& GetHandle () const noexcept;

        #pragma endregion

        #pragma region Operators

        VulkanFence& operator=(VulkanFence const& in_copy) = delete;
        VulkanFence& operator=(VulkanFence&&      in_move) noexcept;

        #pragma endregion
};

END_RUKEN_NAMESPACE