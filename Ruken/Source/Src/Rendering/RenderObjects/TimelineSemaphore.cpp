#include "Rendering/RenderObjects/TimelineSemaphore.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

TimelineSemaphore::TimelineSemaphore(RenderDevice* in_device) noexcept:
    m_device {in_device}
{
    vk::SemaphoreTypeCreateInfo semaphore_type_create_info = {
        .semaphoreType = vk::SemaphoreType::eTimeline,
        .initialValue  = m_value
    };

    vk::SemaphoreCreateInfo semaphore_create_info = {
        .pNext = &semaphore_type_create_info
    };

    if (auto [result, value] = m_device->GetLogicalDevice().createSemaphore(semaphore_create_info); result == vk::Result::eSuccess)
    {
        m_handle = value;
    }
}

TimelineSemaphore::TimelineSemaphore(TimelineSemaphore&& in_move) noexcept:
    m_device {in_move.m_device},
    m_handle {in_move.m_handle},
    m_value  {in_move.m_value}
{
    in_move.m_handle = VK_NULL_HANDLE;
}

TimelineSemaphore::~TimelineSemaphore() noexcept
{
    if (m_handle)
        m_device->GetLogicalDevice().destroy(m_handle);
}

RkUint64 const& TimelineSemaphore::NextValue() noexcept
{
    return ++m_value;
}

RkVoid TimelineSemaphore::Wait() const noexcept
{
    vk::SemaphoreWaitInfo semaphore_wait_info = {
        .semaphoreCount = 1U,
        .pSemaphores    = &m_handle,
        .pValues        = &m_value
    };

    if (m_device->GetLogicalDevice().waitSemaphores(semaphore_wait_info, UINT64_MAX) != vk::Result::eSuccess)
    {
        
    }
}

vk::Semaphore const& TimelineSemaphore::GetHandle() const noexcept
{
    return m_handle;
}

RkUint64 const& TimelineSemaphore::GetValue() const noexcept
{
    return m_value;
}

TimelineSemaphore& TimelineSemaphore::operator=(TimelineSemaphore&& in_move) noexcept
{
    m_device = in_move.m_device;
    m_handle = in_move.m_handle;
    m_value  = in_move.m_value;

    in_move.m_handle = VK_NULL_HANDLE;

    return *this;
}
