#include <fstream>

#include <spirv_cross/spirv_cross.hpp>

#include "Rendering/Resources/Shader.hpp"

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

static std::vector<RkUint32> ReadFile(const std::string& in_filename)
{
    std::ifstream file(in_filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("failed to open file!");

    RkInt64 const file_size = file.tellg();

    std::vector<RkUint32> buffer(file_size / sizeof(RkUint32));

    file.seekg(0);
    file.read(reinterpret_cast<RkChar*>(buffer.data()), file_size);
    file.close();

    return buffer;
}

Shader::Shader(RenderDevice* in_device, std::string_view const in_path) noexcept:
    m_device {in_device}
{
    auto shader_code = ReadFile(in_path.data());

    vk::ShaderModuleCreateInfo shader_module_create_info = {
        .codeSize = shader_code.size() * sizeof(RkUint32),
        .pCode    = shader_code.data()
    };

    m_module = m_device->GetLogicalDevice().createShaderModule(shader_module_create_info).value;

    /*spirv_cross::Compiler const compiler(std::move(shader_code));

    for (auto const& resource : compiler.get_shader_resources())
    {
        RkUint32 set      = compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        RkUint32 binding  = compiler.get_decoration(resource.id, spv::DecorationBinding);
        RkUint32 location = compiler.get_decoration(resource.id, spv::DecorationLocation);
    }*/
}

Shader::~Shader() noexcept
{
    if (m_module)
        m_device->GetLogicalDevice().destroyShaderModule(m_module);
}

RkVoid Shader::Load(ResourceManager& in_manager, ResourceLoadingDescriptor const& in_descriptor)
{
    (void)in_manager;
    (void)in_descriptor;
}

RkVoid Shader::Reload(ResourceManager& in_manager)
{
    (void)in_manager;
}

RkVoid Shader::Unload(ResourceManager& in_manager) noexcept
{
    (void)in_manager;
}

vk::PipelineShaderStageCreateInfo Shader::GetShaderStage() const noexcept
{
    vk::PipelineShaderStageCreateInfo shader_stage_create_info = {
        .stage  = vk::ShaderStageFlagBits::eVertex,
        .module = m_module,
        .pName  = "main"
    };

    return shader_stage_create_info;
}