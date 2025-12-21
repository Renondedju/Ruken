#include "Rendering/Resources/GPUImageView.hpp"

USING_RUKEN_NAMESPACE

GPUImageViewData::GPUImageViewData(vk::raii::Device const& in_owner, vk::ImageViewCreateInfo const& in_create_info):
	IResourceData {},
	image_view   {in_owner, in_create_info}
{}