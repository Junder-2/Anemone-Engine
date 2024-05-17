#include "Filament-Input.slangh"
#include "Filament-Shading.slangh"
#include "Filament-Indirect.slangh"
#include "Filament-Light.slangh"
#include "Filament-Material.slangh"

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

    float3 worldPosition;
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

    output.worldPosition = mul(pushConstants.modelMatrix, float4(v.position, 1)).xyz;

    return output;
}

[shader("fragment")]
float4 fragmentMain(Varyings input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 tangent = normalize(input.tangent);
    float2 uv = input.uv;

    normal = TangentToWorld(normal, tangent, GetTangentSpaceNormal(uv));

    Fragment fragment;
    fragment.position = input.worldPosition;
    fragment.normal = normalize(normal);
    fragment.viewDir = normalize(sceneData.CameraPos.xyz - input.worldPosition);
    fragment.reflected = reflect(-fragment.viewDir, fragment.normal);
    fragment.uv = uv;

    Material material = SetupMaterial(props, fragment, uv);

    float3 shading = 0;
    shading += EvaluateIBL(material, fragment);
    shading += EvaluateDirectionalLight(material, fragment);

    return float4(shading, 1);
}