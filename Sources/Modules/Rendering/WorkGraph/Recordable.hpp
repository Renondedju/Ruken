#pragma once

#include "Types/FundamentalTypes.hpp"

#include <vulkan/vulkan_raii.hpp>

BEGIN_RUKEN_NAMESPACE

// Checks if a type defines TType::Record(vk::raii::CommandBuffer) -> RkVoid
template <typename TType>
concept CRecorder = requires (TType& in_type) { { in_type.Record(std::declval<vk::raii::CommandBuffer>()) } -> std::convertible_to<RkVoid>; };

/// @brief A utility used to store and invoke any type that has a Record(vk::raii::CommandBuffer) member function.
struct Recordable
{
	template <typename TRecorder>
	explicit Recordable(TRecorder& in_recorder_instance) noexcept requires CRecorder<TRecorder>:
		m_instance_ptr	   {std::addressof(in_recorder_instance)},
		m_record_operation {[](RkVoid* in_instance, vk::raii::CommandBuffer const& in_command_buffer) {
			static_cast<TRecorder*>(in_instance)->Record(in_command_buffer);
		}}
	{}

	Recordable()			  				 = default;
	Recordable(const Recordable&) 			 = default;
	Recordable(Recordable&&     ) 			 = default;
	Recordable& operator=(const Recordable&) = default;
	Recordable& operator=(Recordable&&     ) = default;

	RkVoid Record(vk::raii::CommandBuffer const& in_command_buffer) const
	{ m_record_operation(m_instance_ptr, in_command_buffer); }

private:

	RkVoid* m_instance_ptr										         {};
	RkVoid(*m_record_operation)(RkVoid*, vk::raii::CommandBuffer const&) {};
};

END_RUKEN_NAMESPACE