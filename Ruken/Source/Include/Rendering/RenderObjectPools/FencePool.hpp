#pragma once

#include "Rendering/RenderObjectPool.hpp"

BEGIN_RUKEN_NAMESPACE

class FencePool final : public RenderObjectPool<vk::Fence>
{
    private:

        RkVoid Grow() override;

    public:

        #pragma region Constructors

        FencePool(RenderDevice* in_device) noexcept;

        FencePool(FencePool const& in_copy) = delete;
        FencePool(FencePool&&      in_move) = delete;

        ~FencePool() noexcept override;

        #pragma endregion

        #pragma region Methods

        RkVoid Reset() override;

        #pragma endregion

        #pragma region Operators

        FencePool& operator=(FencePool const& in_copy) = delete;
        FencePool& operator=(FencePool&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE