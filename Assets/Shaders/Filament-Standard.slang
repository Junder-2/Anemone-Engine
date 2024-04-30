#include "Filament-Input.slangh"
#include "Filament-Shading.slangh"

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

    Material material = SetupMaterial(props, uv);

    float4 color = GetColor(uv);

    float nDotL = saturate(dot(normal, -sceneData.SunlightDirection.xyz));
    //color.rgb *= nDotL * sceneData.SunlightColor.rgb;
    //color.rgb += sceneData.AmbientColor.rgb;

    float3 shading = EvaluateDirectionalLight(material, fragment);

    // Used for debugging.
    float4 colorMap = props.ColorMap.Sample(uv);
    float3 normalMap = props.NormalMap.Sample(uv).rgb;
    float3 ormMap = props.ORMMap.Sample(uv).rgb;

    float3 outputs[6];
    outputs[0] = shading;
    outputs[1] = shading;
    outputs[2] = shading;
    outputs[3] = color.rgb + shading;
    outputs[4] = color.rgb + shading;
    outputs[5] = color.rgb + shading;

    int outputIndex = int(uv.x * 6 + uv.y + appData.Time * .1);
    //int outputIndex = 2;
    return float4(outputs[outputIndex % 6], 1);
}