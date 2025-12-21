#include "Rendering/Resources/SpirvLoader.hpp"
#include "Rendering/Resources/ShaderModule.hpp"

USING_RUKEN_NAMESPACE

std::vector<std::string_view> SpirvLoader::SupportedExtensions() const noexcept
{
	return { ".spv", ".spirv" };
}

IOTask<ResourcePtr<>> SpirvLoader::Load(LoadContext&& in_context) const
{
	co_return std::make_shared<ShaderModule>(ShaderModule {
		in_context.services, in_context.data
	});
}