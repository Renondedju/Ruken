#pragma once

#include "Rendering/RenderObjectPools/RenderObjectPool.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class RenderTargetPool
{
    private:

        RenderDevice* m_device;

    public:

        #pragma region Constructors

        RenderTargetPool(RenderDevice* in_device) noexcept;

        RenderTargetPool(RenderTargetPool const& in_copy) = delete;
        RenderTargetPool(RenderTargetPool&&      in_move) = delete;

        ~RenderTargetPool() noexcept;

        #pragma endregion

        #pragma region Methods

        

        #pragma endregion

        #pragma region Operators

        RenderTargetPool& operator=(RenderTargetPool const& in_copy) = delete;
        RenderTargetPool& operator=(RenderTargetPool&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE