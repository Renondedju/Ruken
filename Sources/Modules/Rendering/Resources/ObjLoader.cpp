#include "Rendering/Resources/ObjLoader.hpp"
#include "Rendering/Vertex.hpp"

#include "Resources/ResourceManager.hpp"
#include "Resources/MemoryStreamBuffer.hpp"
#include "Resources/GPUMesh.hpp"

#include "Core/Debug/Logging/Logger.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <ranges>

USING_RUKEN_NAMESPACE

std::vector<std::string_view> ObjLoader::SupportedExtensions() const noexcept
{
	return {".obj"};
}

IOTask<ResourcePtr<>> ObjLoader::Load(LoadContext&& in_context) const
{
	RenderDevice* device {in_context.services.LocateService<RenderDevice>()};
	RUKEN_ASSERT(device != nullptr, "Cannot load a mesh without a render device");

	// Converting data buffer to a stream
	MemoryStreamBuffer stream_buffer {&*in_context.data.begin(), &*(in_context.data.end() - 1)};
	std::istream       data_stream   (&stream_buffer);

	// Mesh Data
	std::vector<Vertex>   			 vertices  {};
	std::vector<uint32_t> 			 indices   {};
	tinyobj::attrib_t			     attrib    {};
	std::vector<tinyobj::shape_t>    shapes    {};
	std::vector<tinyobj::material_t> materials {};
	std::string						 warn, err {};

	// Loading model
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &data_stream))
	{
		throw Exception(std::format("An error occurred while trying to load file named {} : {}",
			in_context.file_path.ToString(), warn + err));
	}

	// Checking for warnings
	if (auto const logger {in_context.services.LocateService<Logger>()}; !warn.empty() && logger)
	{
		logger->Warning(ResourceManager::service_name ,"Warning raised while loading file named {} : {}",
			in_context.file_path.ToString(), warn);
	}

	vertices.resize(attrib.vertices.size() / 3);
	for (auto const& [index, vertex] : std::views::enumerate(vertices))
	{
		vertex.position.data[0] = (Meters)attrib.vertices[3 * index + 0];
		vertex.position.data[1] = (Meters)attrib.vertices[3 * index + 1];
		vertex.position.data[2] = (Meters)attrib.vertices[3 * index + 2];
	}

	for (auto const& shape : shapes)
	{
		indices.append_range(shape.mesh.indices | std::views::transform([](const tinyobj::index_t& in_index) {
			return in_index.vertex_index;
		}));
	}

	co_return std::make_shared<GPUMesh>(*device, vertices, indices);
}
