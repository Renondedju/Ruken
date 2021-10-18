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

Shader::Shader(RenderDevice* in_device, std::string_view in_path) noexcept:
    m_device {in_device}
{
    auto vertex_code   = ReadFile(std::string(in_path) + "vert.spv");
    auto fragment_code = ReadFile(std::string(in_path) + "frag.spv");

    vk::ShaderModuleCreateInfo shader_module_create_info = {
        .codeSize = vertex_code.size() * sizeof(RkUint32),
        .pCode    = vertex_code.data()
    };

    m_vertex_module = m_device->GetLogicalDevice().createShaderModule(shader_module_create_info).value;

    shader_module_create_info.codeSize = fragment_code.size() * sizeof(RkUint32);
    shader_module_create_info.pCode    = fragment_code.data();

    m_fragment_module = m_device->GetLogicalDevice().createShaderModule(shader_module_create_info).value;

    /*spirv_cross::Compiler vertex_compiler(std::move(vertex_code));

    auto vertex_resources = vertex_compiler.get_shader_resources();

    for (auto const& resource : vertex_resources.sampled_images)
    {
        RkUint32 set      = vertex_compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        RkUint32 binding  = vertex_compiler.get_decoration(resource.id, spv::DecorationBinding);
        RkUint32 location = vertex_compiler.get_decoration(resource.id, spv::DecorationLocation);
    }

    spirv_cross::Compiler fragment_compiler(std::move(fragment_code));

    auto fragment_resources = fragment_compiler.get_shader_resources();

    for (auto const& resource : fragment_resources.sampled_images)
    {
        RkUint32 set      = fragment_compiler.get_decoration(resource.id, spv::DecorationDescriptorSet);
        RkUint32 binding  = fragment_compiler.get_decoration(resource.id, spv::DecorationBinding);
        RkUint32 location = fragment_compiler.get_decoration(resource.id, spv::DecorationLocation);
    }*/
}

Shader::~Shader() noexcept
{
    m_device->GetLogicalDevice().destroyShaderModule(m_vertex_module);
    m_device->GetLogicalDevice().destroyShaderModule(m_fragment_module);
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

std::vector<vk::PipelineShaderStageCreateInfo> Shader::GetShaderStages() const
{
    std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_create_infos;

    if (m_vertex_module)
    {
        shader_stage_create_infos.push_back(
        {
            .stage  = vk::ShaderStageFlagBits::eVertex,
            .module = m_vertex_module,
            .pName  = "main"
        });
    }

    if (m_fragment_module)
    {
        shader_stage_create_infos.push_back(
        {
            .stage  = vk::ShaderStageFlagBits::eFragment,
            .module = m_fragment_module,
            .pName  = "main"
        });
    }

    return shader_stage_create_infos;
}
