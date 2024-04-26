#include "Filament-Input.include"

// Per vertex data.
/*struct Attributes
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 color : COLOR;
};*/

// TODO: Tangents
struct Vertex
{
    float3 position;
    float uvX;
    float3 normal;
    float uvY;
    float4 color;
};

struct PushConstants
{
    matrix<float, 4, 4> renderMatrix;
    matrix<float, 4, 4> modelMatrix;

    // Similar to a GLSL buffer_reference.
    // Uses uint64 pointer manipulation to index, hence the GL_EXT_shader_explicit_arithmetic_types_int64 dependency.
    ConstBufferPointer<Vertex> vertices;
};

// Data passed from vertex to the next stage.
struct Varyings
{
    float4 position : SV_Position;
    float3 normal;
    float3 color;
    float2 uv;
};

// Equivalent to GLSL push constants.
[[vk::push_constant]]
ConstantBuffer<PushConstants> pushConstants;

ParameterBlock<AppData> appData;
ParameterBlock<SceneData> sceneData;
ParameterBlock<MaterialProperties> props;

[shader("vertex")]
Varyings vertexMain(uint vid : SV_VertexID)
{
    Varyings output;

    Vertex v = pushConstants.vertices[vid];
    output.position = mul(pushConstants.renderMatrix, float4(v.position, 1));
    output.normal = mul(float3x3(pushConstants.modelMatrix), v.normal);
    output.color = 1;
    output.uv = float2(v.uvX, v.uvY);

    return output;
}

float4 texture(TextureSamplerPair pair, float2 uv)
{
    return pair.T.Sample(pair.S, uv);
}

[shader("fragment")]
float4 fragmentMain(Varyings input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float2 uv = input.uv;

    float4 color = props.ColorMap.T.Sample(props.ColorMap.S, uv) * float4(input.color, 1);

    float nDotL = saturate(dot(normal, -sceneData.SunlightDirection.rgb));
    color.rgb *= nDotL * sceneData.SunlightColor.rgb;
    color.rgb += sceneData.AmbientColor.rgb;

    return float4(color);
}