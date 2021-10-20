#pragma once

#include "Rendering/RenderObjectPools/RenderObjectPool.hpp"

BEGIN_RUKEN_NAMESPACE

class SemaphorePool final : public RenderObjectPool<vk::Semaphore>
{
    private:

        RkVoid Grow() override;

    public:

        #pragma region Constructors

        SemaphorePool(RenderDevice* in_device) noexcept;

        SemaphorePool(SemaphorePool const& in_copy) = delete;
        SemaphorePool(SemaphorePool&&      in_move) = delete;

        ~SemaphorePool() noexcept override;

        #pragma endregion

        #pragma region Methods

        RkVoid Reset() override;

        #pragma endregion

        #pragma region Operators

        SemaphorePool& operator=(SemaphorePool const& in_copy) = delete;
        SemaphorePool& operator=(SemaphorePool&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE