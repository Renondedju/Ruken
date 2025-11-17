#pragma once

#include "Core/Build/Namespace.hpp"
#include "Core/Debug/SourceLocation.hpp"
#include "Core/Types/FundamentalTypes.hpp"

#include <string_view>
#include <vulkan/vulkan.hpp>
#include <tracy/TracyVulkan.hpp>

#ifdef RUKEN_TRACE_BUILD

#include <unordered_map>
#include <shared_mutex>

#else
	namespace tracy
	{ using SourceLocationData = void; }
#endif

BEGIN_RUKEN_NAMESPACE

/// @brief A collection of functions and members used by the GPUTasks to properly integrate the tracy profiler
struct TracyVkUtilities
{
	tracy::SourceLocationData* GetOrInsertSourceLocationData(std::source_location const& in_source_location,
		const char* in_name = nullptr, uint32_t in_color = 0) noexcept;

	/**
	 * Starts a tracy vulkan zone.
	 * @param in_context Tracy vulkan context.
	 * @param in_command_buffer Command buffer to trace.
	 * @param in_source_location Source location of the start of the zone
	 * @param in_name Name of the zone
	 * @param in_color Color of the zone
	 * @param in_active True if the zone is active, false otherwise
	 * @returns Zone context
	 */
	static std::unique_ptr<tracy::VkCtxScope> TracyVulkanZone(TracyVkCtx in_context, vk::CommandBuffer in_command_buffer,
		std::source_location const& in_source_location, std::string_view in_name, uint32_t in_color, bool in_active) noexcept;

	static std::unique_ptr<tracy::VkCtxScope> TracyVulkanZone(TracyVkCtx in_context, vk::CommandBuffer in_command_buffer,
		std::source_location const& in_source_location, std::string_view in_name, bool in_active) noexcept;

	static std::unique_ptr<tracy::VkCtxScope> TracyVulkanZone(TracyVkCtx in_context, vk::CommandBuffer in_command_buffer,
		std::source_location const& in_source_location, uint32_t in_color, bool in_active) noexcept;

	static std::unique_ptr<tracy::VkCtxScope> TracyVulkanZone(TracyVkCtx in_context, vk::CommandBuffer in_command_buffer,
		std::source_location const& in_source_location, bool in_active) noexcept;

	static std::unique_ptr<tracy::VkCtxScope> TracyVulkanZone(TracyVkCtx in_context, vk::CommandBuffer in_command_buffer,
		tracy::SourceLocationData const* in_source_data, bool in_active) noexcept;


	private:

		#ifdef RUKEN_TRACE_BUILD

		std::shared_mutex												    m_source_location_mutex    {};
		std::unordered_map<std::source_location, tracy::SourceLocationData> m_source_location_registry {};

		#endif
};

static inline TracyVkUtilities s_tracy_vk_utilities {};

END_RUKEN_NAMESPACE