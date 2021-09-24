#include "Rendering/Resources/Shader.hpp"

#include <fstream>

#include "Rendering/RenderDevice.hpp"

USING_RUKEN_NAMESPACE

static std::vector<RkChar> ReadFile(const std::string& in_filename)
{
    std::ifstream file(in_filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("failed to open file!");

    size_t fileSize = file.tellg();

    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

Shader::Shader(RenderDevice* in_device, std::string_view in_path) noexcept: DeviceObjectBase(in_device)
{
    auto vertex_code   = ReadFile(std::string(in_path) + "vert.spv");
    auto fragment_code = ReadFile(std::string(in_path) + "frag.spv");

    vk::ShaderModuleCreateInfo shader_module_create_info = {
        .codeSize = vertex_code.size(),
        .pCode    = reinterpret_cast<const uint32_t*>(vertex_code.data())
    };

    m_vertex_module = m_device->GetLogicalDevice().createShaderModule(shader_module_create_info).value;

    shader_module_create_info.codeSize = fragment_code.size();
    shader_module_create_info.pCode    = reinterpret_cast<const uint32_t*>(fragment_code.data());

    m_fragment_module = m_device->GetLogicalDevice().createShaderModule(shader_module_create_info).value;
}

Shader::~Shader() noexcept
{
    m_device->GetLogicalDevice().destroyShaderModule(m_vertex_module);
    m_device->GetLogicalDevice().destroyShaderModule(m_fragment_module);
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