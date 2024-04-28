#include "Filament-Input.include"

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
    float3 tangent;
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
    output.tangent = mul(float3x3(pushConstants.modelMatrix), v.tangent.xyz);
    output.color = 1;
    output.uv = float2(v.uvX, v.uvY);

    return output;
}

float3 TangentDirToWorld(float3 normal, float3 tangent, float3 dir)
{
    float3 binormal = cross(normal, tangent);
    return normalize(
        dir.x * tangent +
        dir.y * binormal +
        dir.z * normal);
}

[shader("fragment")]
float4 fragmentMain(Varyings input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 tangent = normalize(input.tangent);
    float2 uv = input.uv;

    float4 colorMap = props.ColorMap.Sample(uv);
    float3 normalMap = props.NormalMap.Sample(uv).rgb;
    float3 ormMap = props.ORMMap.Sample(uv).rgb;
    normal = TangentDirToWorld(normal, tangent, GetTangentSpaceNormal(uv));

    float4 color = GetColor(uv);
    
    float nDotL = saturate(dot(normal, -sceneData.SunlightDirection.xyz));
    color.rgb *= nDotL * sceneData.SunlightColor.rgb;
    color.rgb += sceneData.AmbientColor.rgb;

    return float4(color);
}