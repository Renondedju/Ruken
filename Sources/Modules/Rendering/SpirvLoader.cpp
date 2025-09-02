#include "Rendering/SpirvLoader.hpp"
#include "Rendering/ShaderModule.hpp"

USING_RUKEN_NAMESPACE

std::vector<std::string_view> SpirvLoader::SupportedExtensions() const noexcept
{
	return { ".spv", ".spirv" };
}

IOTask<ResourcePtr<>> SpirvLoader::Load(LoadContext const& in_context) const
{
	co_return std::make_shared<ShaderModule>(ShaderModule {
		in_context.services, in_context.file_path, in_context.data
	});
}