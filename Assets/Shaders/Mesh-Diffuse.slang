#include "Mesh-Input.slangh"

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
    float4 position : SV_Position;
    float3 normal;
    float3 color;
    float2 uv;
};

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
    float2 uv = input.uv;

    float4 color = float4(input.color, 1);

    float nDotL = saturate(dot(normal, -sceneData.SunlightDirection.rgb));
    color.rgb *= nDotL * sceneData.SunlightColor.rgb;
    color.rgb += sceneData.AmbientColor.rgb;

    return float4(color);
}