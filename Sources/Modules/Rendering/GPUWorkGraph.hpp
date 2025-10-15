#pragma once

#include "Rendering/RenderDevice.hpp"
#include "Rendering/GpuFence.hpp"

#include <vulkan/vulkan.hpp>

BEGIN_RUKEN_NAMESPACE

struct GPUImageAccess
{
	vk::Image				  image				{};
	vk::ImageLayout			  layout 		    {vk::ImageLayout		   ::eUndefined};
	vk::PipelineStageFlags2   stages 		    {vk::PipelineStageFlagBits2::eTopOfPipe};
	vk::AccessFlags2          access_flags      {};
	vk::ImageSubresourceRange subresource_range {};

	RkBool operator==(GPUImageAccess const& in_other) const noexcept
	{
		return
			image             == in_other.image        &&
			layout            == in_other.layout       &&
			stages            == in_other.stages       &&
			access_flags      == in_other.access_flags &&
			subresource_range == in_other.subresource_range;
	}
};

struct GPUBufferAccess
{
	vk::Buffer				buffer		 {};
	vk::PipelineStageFlags2 stages 		 {vk::PipelineStageFlagBits2::eTopOfPipe};
	vk::AccessFlags2        access_flags {};
	vk::DeviceSize			offset		 {};
	vk::DeviceSize			size         {};

	RkBool operator==(GPUBufferAccess const& in_other) const noexcept
	{
		return
			buffer       == in_other.buffer       &&
			stages       == in_other.stages       &&
			access_flags == in_other.access_flags &&
			offset		 == in_other.offset		  &&
			size		 == in_other.size;
	}
};

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

struct GPUWorkNode
{
	Recordable					 record_callback;
	std::vector<GPUImageAccess>  image_accesses;
	std::vector<GPUBufferAccess> buffer_accesses;
};

struct GPUWorkGraph
{
	#pragma region Lifetime

	explicit GPUWorkGraph(RenderDevice& in_owner) noexcept;
	GPUWorkGraph		   (GPUWorkGraph&&)		 = delete;
	GPUWorkGraph		   (const GPUWorkGraph&) = delete;
	GPUWorkGraph& operator=(GPUWorkGraph&&)	     = delete;
	GPUWorkGraph& operator=(const GPUWorkGraph&) = delete;
	~GPUWorkGraph();

	#pragma endregion

	#pragma region Methods

	RkVoid AddPass(GPUWorkNode&& in_node) noexcept;

	DynamicTask<> Submit(vk::Semaphore in_wait_semaphore, vk::Semaphore in_signal_semaphore) const noexcept;

	static RkVoid PipelineBarrier(vk::raii::CommandBuffer const& in_buffer, GPUImageAccess  const& in_from, GPUImageAccess  const& in_to) noexcept;
	static RkVoid PipelineBarrier(vk::raii::CommandBuffer const& in_buffer, GPUBufferAccess const& in_from, GPUBufferAccess const& in_to) noexcept;

	#pragma endregion

	private:

		#pragma region Members

		RenderDevice&			 m_owner;
		std::vector<GPUWorkNode> m_instructions;

		#pragma endregion
};

END_RUKEN_NAMESPACE