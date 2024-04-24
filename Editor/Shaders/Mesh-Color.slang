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
    matrix<float, 4, 4> model_matrix;

    // Similar to a GLSL buffer_reference.
    // Uses uint64 pointer manipulation to index, hence the GL_EXT_shader_explicit_arithmetic_types_int64 dependency.
    ConstBufferPointer<Vertex> vertices;
};

struct AppData
{
    float Time;
};

struct SceneData
{
    float4 AmbientColor;

    float4 SunlightDirection;
    float4 SunlightColor;
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
    float3 normal;
    float3 color;
    float2 uv;
};

// Per-material data.
struct Material
{
    Texture2D texture;
    SamplerState sampler;
};

ParameterBlock<AppData> appData;
ParameterBlock<SceneData> sceneData;
ParameterBlock<Material> material;

[shader("vertex")]
Varyings vertexMain(uint vid : SV_VertexID)
{
    Varyings output;

    Vertex v = vertices[vid];
    output.sv_position = mul(render_matrix, float4(v.position, 1));
    output.normal = mul(float3x3(model_matrix), v.normal);
    output.color = 1;
    output.uv = float2(v.uv_x, v.uv_y);

    return output;
}

[shader("fragment")]
float4 fragmentMain(Varyings input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float2 uv = input.uv;

    float4 color = material.texture.Sample(material.sampler, uv) * float4(input.color, 1);

    float nDotL = saturate(dot(normal, -sceneData.SunlightDirection.rgb));
    color.rgb *= nDotL * sceneData.SunlightColor.rgb;
    color.rgb += sceneData.AmbientColor.rgb;

    return float4(color);
}