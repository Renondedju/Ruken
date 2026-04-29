import Camera;

struct VertexInput
{
    float3 position;
};

struct VertexOutput
{
    float3 local_position;
    float4 sv_position : SV_Position;
    float3 color;
};

layout(set=0, binding=0) StructuredBuffer<float4x4>   instance_data;
layout(set=1, binding=0) ParameterBlock  <CameraData> camera_data;

[shader("vertex")]
VertexOutput vertMain(VertexInput in_vertex, uint in_instance_id : SV_InstanceID)
{
    VertexOutput output;
    float4x4     model = instance_data[in_instance_id];

    output.local_position = in_vertex.position.xyz;
    output.sv_position    = mul(camera_data.projection, mul(camera_data.view, mul(model, float4(in_vertex.position, 1.0))));

    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput in_vertex) : SV_Target
{
    return float4(in_vertex.local_position / 2.0f + 0.5f, 1.0);
}