#version 450

#extension GL_EXT_nonuniform_qualifier : require

struct MaterialData
{
    uint albedoTexture;
    uint normalTexture;
};

struct DrawData
{
    uint materialIndex;
    uint transformIndex;
};

layout(set = 0, binding = 0) readonly buffer DrawBuffer {
    DrawData drawData[];
};

layout(set = 0, binding = 2) readonly buffer MaterialBuffer {
    MaterialData materialData[];
};

layout(set = 0, binding = 3) uniform texture2D materialTextures[];

layout(set = 2, binding = 0) uniform sampler texSampler;

layout(push_constant) uniform ObjectData {
    uint index;
} objectData;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

void main()
{
    DrawData     dd = drawData[objectData.index];
    MaterialData md = materialData[dd.materialIndex];

    outColor = vec4(fragColor * texture(sampler2D(materialTextures[md.albedoTexture], texSampler), fragUV).rgb, 1.0);
}