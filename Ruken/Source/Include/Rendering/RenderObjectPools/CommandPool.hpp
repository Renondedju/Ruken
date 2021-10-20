#pragma once

#include "Rendering/RenderObjectPools/RenderObjectPool.hpp"

BEGIN_RUKEN_NAMESPACE

class CommandPool final : public RenderObjectPool<vk::CommandBuffer>
{
    private:

        vk::CommandPool m_handle;

        RkVoid Grow() override;

    public:

        #pragma region Constructors

        CommandPool(RenderDevice* in_device, RkUint32 in_family_index, vk::CommandPoolCreateFlags in_flags = {}) noexcept;

        CommandPool(CommandPool const& in_copy) = delete;
        CommandPool(CommandPool&&      in_move) = delete;

        ~CommandPool() noexcept override;

        #pragma endregion

        #pragma region Methods

        RkVoid Reset() override;

        #pragma endregion

        #pragma region Operators

        CommandPool& operator=(CommandPool const& in_copy) = delete;
        CommandPool& operator=(CommandPool&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE