#pragma once

#include "Rendering/RenderDefines.hpp"

BEGIN_RUKEN_NAMESPACE

class RenderDevice;

class TimelineSemaphore final
{
    private:

        RenderDevice* m_device {nullptr};

        vk::Semaphore m_handle {};
        RkUint64      m_value  {0ULL};

    public:

        #pragma region Constructors

        TimelineSemaphore(RenderDevice* in_device) noexcept;

        TimelineSemaphore(TimelineSemaphore const& in_copy) = delete;
        TimelineSemaphore(TimelineSemaphore&&      in_move) = delete;

        ~TimelineSemaphore() noexcept;

        #pragma endregion

        #pragma region Methods

        RkUint64 const& NextValue() noexcept;
        RkVoid          Wait     () const noexcept;

        vk::Semaphore const& GetHandle() const noexcept;
        RkUint64      const& GetValue () const noexcept;

        #pragma endregion

        #pragma region Operators

        TimelineSemaphore& operator=(TimelineSemaphore const& in_copy) = delete;
        TimelineSemaphore& operator=(TimelineSemaphore&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE