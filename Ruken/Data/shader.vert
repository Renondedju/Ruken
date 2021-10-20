#version 450

struct TransformData
{
    mat4 model;
};

struct DrawData
{
    uint materialIndex;
    uint transformIndex;
};

layout(set = 0, binding = 0) readonly buffer DrawBuffer {
    DrawData drawData[];
};

layout(set = 0, binding = 1) readonly buffer TransformBuffer {
    TransformData transformData[];
};

layout(set = 1, binding = 0) uniform CameraData {
    mat4 view;
    mat4 proj;
} cameraData;

layout(push_constant) uniform ObjectData {
    uint index;
} objectData;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragUV;

void main()
{
    DrawData      dd = drawData[objectData.index];
    TransformData td = transformData[dd.transformIndex];

    gl_Position = cameraData.proj * cameraData.view * td.model * vec4(inPosition, 1.0);
	
    fragColor = inColor;
	fragUV    = inUV;
}