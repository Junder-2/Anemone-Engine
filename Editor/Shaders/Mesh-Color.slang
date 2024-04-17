struct Vertex
{
    float3 position;
    float uv_x;
    float3 normal;
    float uv_y;
    float4 color;
};

// Equivalent to GLSL push constants.
[[vk::push_constant]]
cbuffer Constants
{
    matrix<float, 4, 4> render_matrix;

    // Similar to a GLSL buffer_reference.
    // Uses uint64 pointer manipulation to index, hence the GL_EXT_shader_explicit_arithmetic_types_int64 dependency.
    ConstBufferPointer<Vertex> vertices;
};

// Per vertex data.
/*struct Attributes
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 color : COLOR;
};*/

// Data passed from vertex to the next stage.
struct Varyings
{
    float4 sv_position : SV_Position;
    float3 color;
    float2 uv;
};

[shader("vertex")]
Varyings vertexMain(uint vid : SV_VertexID)
{
    Varyings output;

    Vertex v = vertices[vid];
    output.sv_position = mul(render_matrix, float4(v.position, 1));
    output.color = v.color.rgb;
    output.uv = float2(v.uv_x, v.uv_y);

    return output;
}

[shader("fragment")]
float4 fragmentMain(Varyings input) : SV_TARGET
{
    return float4(input.color, 1);
}