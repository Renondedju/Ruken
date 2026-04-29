module Camera;

public struct CameraData
{
    public float3   world_position;

    public float4x4 view;
    public float4x4 projection;
    public float4x4 view_projection;

    public float4x4 inverted_view;
    public float4x4 inverted_projection;
    public float4x4 inverted_view_projection;
};