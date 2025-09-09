#include "Rendering/Resources/ImageView.hpp"

USING_RUKEN_NAMESPACE

GPUImageView::GPUImageView(vk::raii::Device const& in_owner, vk::ImageViewCreateInfo const& in_create_info):
	Resource   {},
	image_view {in_owner, in_create_info}
{}