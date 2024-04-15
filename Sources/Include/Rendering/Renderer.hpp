
#pragma once

#include <vector>

#include "Meta/Meta.hpp"
#include "Core/Service.hpp"

#include "Rendering/RenderContext.hpp"

#include "Vulkan/Core/VulkanDevice.hpp"
#include "Vulkan/Core/VulkanInstance.hpp"
#include "Vulkan/Core/VulkanPhysicalDevice.hpp"
#include "Vulkan/Utilities/VulkanDeviceAllocator.hpp"

BEGIN_RUKEN_NAMESPACE

class Logger;
class Window;
class Scheduler;

class Renderer final : public Service<Renderer>
{
    private:

        #pragma region Members

        Logger*    m_logger    {nullptr};
        Scheduler* m_scheduler {nullptr};

        std::unique_ptr<VulkanInstance>        m_instance         {};
        std::unique_ptr<VulkanPhysicalDevice>  m_physical_device  {};
        std::unique_ptr<VulkanDevice>          m_device           {};
        std::unique_ptr<VulkanDeviceAllocator> m_device_allocator {};
        std::vector    <RenderContext>         m_render_contexts  {};

        #pragma endregion

        #pragma region Methods

        RkVoid MakeContext(Window& in_window) noexcept;

        #pragma endregion

    public:

        #pragma region Members

        // Static name of the service, used by the kernel to report service errors
        constexpr static const RkChar* service_name = RUKEN_STRING(Renderer);

        #pragma endregion

        #pragma region Constructors

        Renderer(ServiceProvider& in_service_provider) noexcept;

        Renderer(Renderer const& in_copy) = delete;
        Renderer(Renderer&&      in_move) = delete;
        ~Renderer() noexcept;

        #pragma endregion

        #pragma region Methods

        [[nodiscard]] VulkanInstance&        GetInstance       () const noexcept;
        [[nodiscard]] VulkanPhysicalDevice&  GetPhysicalDevice () const noexcept;
        [[nodiscard]] VulkanDevice&          GetDevice         () const noexcept;
        [[nodiscard]] VulkanDeviceAllocator& GetDeviceAllocator() const noexcept;

        #pragma endregion

        #pragma region Operators

        Renderer& operator=(Renderer const& in_copy) = delete;
        Renderer& operator=(Renderer&&      in_move) = delete;

        #pragma endregion
};

END_RUKEN_NAMESPACE