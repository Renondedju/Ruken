#include "Rendering/Coroutines/TracyVkUtilities.hpp"
#include "Resources/GPUBuffer.hpp"

USING_RUKEN_NAMESPACE

tracy::SourceLocationData* TracyVkUtilities::GetOrInsertSourceLocationData(
	std::source_location const& in_source_location,
	const    char*              in_name,
	uint32_t const              in_color) noexcept
{
#ifdef TRACY_ENABLE
	{
		std::shared_lock shared_lock(m_source_location_mutex);

		if (m_source_location_registry.contains(in_source_location))
			return &m_source_location_registry.at(in_source_location);
	}

	std::unique_lock unique_lock(m_source_location_mutex);

	m_source_location_registry.try_emplace(in_source_location, tracy::SourceLocationData {
		in_name, in_source_location.function_name(), in_source_location.file_name(), in_source_location.line(), in_color
	});

	return &m_source_location_registry.at(in_source_location);
#else
	return nullptr;
#endif
}

std::unique_ptr<tracy::VkCtxScope> TracyVkUtilities::TracyVulkanZone(
	TracyVkCtx			 const  in_context,
	vk::CommandBuffer    const  in_command_buffer,
	std::source_location const& in_source_location,
	std::string_view	 const  in_name,
	uint32_t			 const  in_color,
	bool				 const  in_active) noexcept
{
	return TracyVulkanZone(in_context, in_command_buffer,
		::RUKEN_NAMESPACE::s_tracy_vk_utilities.GetOrInsertSourceLocationData(
			in_source_location, in_name.data(), in_color
		), in_active);
}

std::unique_ptr<tracy::VkCtxScope> TracyVkUtilities::TracyVulkanZone(
	TracyVkCtx			 const  in_context,
	vk::CommandBuffer    const  in_command_buffer,
	std::source_location const& in_source_location,
	std::string_view	 const  in_name,
	bool				 const  in_active) noexcept
{
	return TracyVulkanZone(in_context, in_command_buffer,
		::RUKEN_NAMESPACE::s_tracy_vk_utilities.GetOrInsertSourceLocationData(
			in_source_location, in_name.data()
		), in_active);
}

std::unique_ptr<tracy::VkCtxScope> TracyVkUtilities::TracyVulkanZone(
	TracyVkCtx			 const  in_context,
	vk::CommandBuffer    const  in_command_buffer,
	std::source_location const& in_source_location,
	uint32_t			 const  in_color,
	bool				 const  in_active) noexcept
{
	return TracyVulkanZone(in_context, in_command_buffer,
		::RUKEN_NAMESPACE::s_tracy_vk_utilities.GetOrInsertSourceLocationData(
			in_source_location, nullptr, in_color
		), in_active);
}

std::unique_ptr<tracy::VkCtxScope> TracyVkUtilities::TracyVulkanZone(
	TracyVkCtx			 const  in_context,
	vk::CommandBuffer    const  in_command_buffer,
	std::source_location const& in_source_location,
	bool				 const  in_active) noexcept
{
	return TracyVulkanZone(in_context, in_command_buffer,
		::RUKEN_NAMESPACE::s_tracy_vk_utilities.GetOrInsertSourceLocationData(
			in_source_location, nullptr
		), in_active);
}

std::unique_ptr<tracy::VkCtxScope> TracyVkUtilities::TracyVulkanZone(
	TracyVkCtx        		  const  in_context,
	vk::CommandBuffer 		  const  in_command_buffer,
	tracy::SourceLocationData const* in_source_data,
	bool					  const  in_active) noexcept
{
#ifdef TRACY_ENABLE
	return std::make_unique<tracy::VkCtxScope>(in_context, in_source_data, in_command_buffer, TRACY_CALLSTACK, in_active);
#else
	return std::make_unique<tracy::VkCtxScope>();
#endif
}
