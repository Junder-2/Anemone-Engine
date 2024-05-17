#include "Mesh-Input.slangh"

struct Varyings
{
    float4 position : SV_Position;
    float3 normal;
    float3 color;
    float2 uv;
};

struct SkyData
{
    SamplerCube Skybox;
};

ParameterBlock<SkyData> skyData;

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

[shader("fragment")]
float4 fragmentMain(Varyings input) : SV_TARGET
{
    float3 normal = normalize(input.normal);

    float4 skyColor = skyData.Skybox.Sample(normal);

    return skyColor;
}