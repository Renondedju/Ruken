///
/// Infinite grid plane shader, inspired from Blender's grid.
///
/// This shader contains its own geometry and does not need vertices/indices buffers.
/// Topology type used to draw should be VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
/// and vkCmdDraw command should draw 6 vertices.
/// Color blending should be enabled as well.
///

import Camera;

struct VertexOutput
{
    float3 ray_origin;
    float3 ray_direction;

    float4 sv_position : SV_Position;
};

struct FragmentOutput
{
    float4 color : SV_Target;
    float  depth : SV_Depth;
};

struct GridParameters
{
    float  grid_opacity = 1.0;
    float  axis_opacity = 1.0;

    // plane_normal is expected to be one of the origin vectors
    float3 plane_normal = float3(0.0, 1.0, 0.0);
    float3 x_axis_color;
    float3 y_axis_color;
    float3 z_axis_color;
};

// TODO: Support automatic memory layout ?
layout(set=0, binding=0) ParameterBlock<GridParameters> grid_parameters;
layout(set=1, binding=0) ParameterBlock<CameraData>     camera_data;

// TODO: Should live in a "geometry" module
float3 RayPlaneIntersection(float3 in_ray_origin, float3 in_ray_direction, float3 in_plane_normal)
{
    float3 normal = abs(in_plane_normal);
    float  t      = dot(in_ray_origin, normal) / dot(normal, in_ray_direction);

    return in_ray_origin - (t * in_ray_direction);
}

/**
 * @brief Swizzles a point based on the plane normal.
 * @param in_point Point to project to the grid.
 */
float2 SwizzleFromNormal(float3 in_point)
{
    if (abs(grid_parameters.plane_normal.x) == 1.0) return in_point.yz;
    if (abs(grid_parameters.plane_normal.y) == 1.0) return in_point.xz;
    if (abs(grid_parameters.plane_normal.z) == 1.0) return in_point.xy;

    return in_point.xz;
}

/**
 * @brief Returns the color of the grid based on the world position of the current fragment.
 * @param in_position World position of the fragment.
 * @param in_scale    Scale of the grid in meters.
 * @param in_opacity  Opacity of the grid.
 */
float4 Grid(float3 in_position, float in_scale, float in_opacity)
{
    float2 coord = SwizzleFromNormal(in_position) / in_scale;
    float2 grid  = abs(fract(coord - 0.5) - 0.5);
    float2 dist  = smoothstep(float2(0.0), fwidth(coord), grid);
    float  line  = min(dist.x, dist.y);

    return float4(float3(1.0), (1.0 - min(line, 1.0))) * in_opacity;
}

/**
 * @brief Returns the color of the origin axes based on the world position of the current fragment.
 * @param in_position  World position of the fragment.
 * @param in_thickness Thickness of the origin axes.
 */
float4 OriginAxes(float3 in_position, float in_thickness)
{
    float2 position   = SwizzleFromNormal(in_position);
    float2 derivative = fwidth(position * in_thickness);

    // Computing the distance to the axes
    float2 grid = abs(position);
    float2 dist = smoothstep(float2(0.0), derivative, grid);
    float  line = min(dist.x, dist.y);

    float4 color   = float4(0.0);
    float2 minimum = min(derivative, float2(1.0));

    float3 color_a = grid_parameters.x_axis_color;
    float3 color_b = grid_parameters.z_axis_color;

    if      (abs(grid_parameters.plane_normal.x) == 1.0) { color_a = grid_parameters.y_axis_color; color_b = grid_parameters.z_axis_color; }
    else if (abs(grid_parameters.plane_normal.y) == 1.0) { color_a = grid_parameters.x_axis_color; color_b = grid_parameters.z_axis_color; }
    else if (abs(grid_parameters.plane_normal.z) == 1.0) { color_a = grid_parameters.x_axis_color; color_b = grid_parameters.y_axis_color; }

    if (position.y > -minimum.y && position.y < minimum.y) color = float4(color_a, 1.0);
    if (position.x > -minimum.x && position.x < minimum.x) color = float4(color_b, 1.0);

    return float4(color.rgb, (1.0 - min(line, 1.0)) * color.a);
}

[shader("vertex")]
VertexOutput vertMain(uint in_vertex_id : SV_VertexID)
{
    float2 positions[] = {
        float2(-1.0, -1.0),
        float2( 1.0, -1.0),
        float2( 1.0,  1.0),

        float2(-1.0, -1.0),
        float2( 1.0,  1.0),
        float2(-1.0,  1.0),
    };

    // Computing near and far plane world positions for each pixel of the viewport.
    float2 position  = positions[in_vertex_id];
    float4 ray_start = mul(camera_data.inverted_view_projection, float4(position, 0.0, 1.0));
    float4 ray_end   = mul(camera_data.inverted_view_projection, float4(position, 1.0, 1.0));

    ray_start /= ray_start.w;
    ray_end   /= ray_end  .w;

    VertexOutput output;

    output.ray_origin    = ray_start.xyz;
    output.ray_direction = ray_end.xyz - ray_start.xyz;
    output.sv_position   = float4(position, 0.0, 1.0);

    return output;
}

[shader("fragment")]
FragmentOutput fragMain(VertexOutput in_vertex)
{
    // 1. Computing view ray-plane intersection position
    float3 world_position = RayPlaneIntersection(in_vertex.ray_origin, in_vertex.ray_direction, grid_parameters.plane_normal);
    float4 ndc_position   = mul(camera_data.view_projection, float4(world_position, 1.0));
    float  grid_depth     = (ndc_position /= ndc_position.w).z;

    // 2. Fading
    float3 camera_to_fragment = normalize(world_position - in_vertex.ray_origin);
    float  view_angle         = abs(dot(camera_to_fragment, grid_parameters.plane_normal));
    float  fading             = smoothstep(0.0, 0.3, view_angle) * smoothstep(1.0, 0.9999, grid_depth);

    if (abs(grid_parameters.plane_normal.x) == 1.0) fading *= smoothstep(0.0, 0.5, abs(camera_data.world_position.x));
    if (abs(grid_parameters.plane_normal.y) == 1.0) fading *= smoothstep(0.0, 0.5, abs(camera_data.world_position.y));
    if (abs(grid_parameters.plane_normal.z) == 1.0) fading *= smoothstep(0.0, 0.5, abs(camera_data.world_position.z));

    // 3. Grid geometry + color
    float4 axes  = OriginAxes(world_position, 2.0);
    float  power = max(log10(length(camera_data.world_position)) - 0.5, 0.0);
    float4 grid  = Grid(abs(world_position), pow(10.0, floor(power)), 1.0 - fract(power))
                 + Grid(abs(world_position), pow(10.0,  ceil(power)),       fract(power));

    // 4. Output
    FragmentOutput output;

    float4 grid_color = float4(grid.rgb, grid.a * fading * grid_parameters.grid_opacity);
    float4 axes_color = float4(axes.rgb, axes.a * fading * grid_parameters.axis_opacity);

    output.depth = grid_depth;
    output.color = axes_color.a > grid_color.a ?
        axes_color :
        axes_color + (grid_color * (1.0 - axes_color.a));

    if(output.color.a == 0.0)
        discard;

    // Gamma correction
    output.color.rgb = pow(output.color.rgb, 2.2);

    return output;
}