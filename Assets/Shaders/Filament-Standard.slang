#include "Filament-Input.slangh"
#include "Filament-BRDF.slangh"
#include "Filament-Material.slangh"
#include "Filament-Fragment.slangh"
#include "Filament-Light.slangh"

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

float3 TangentDirToWorld(float3 normal, float3 tangent, float3 dir)
{
    float3 binormal = cross(normal, tangent);
    return normalize(
        dir.x * tangent +
        dir.y * binormal +
        dir.z * normal);
}

static float3 d1, d2, d3;

float3 IsotropicLobe(Material material, Light light, float3 halfDir,
    float NoV, float NoL, float NoH, float LoH)
{
    float D = Distribution(material.roughness, NoH, halfDir);
    float V = Visibility(material.roughness, NoV, NoL);
    float3 F = Fresnel(material.f0, LoH);

    return (D * V) * F;
}

float3 SpecularLobe(
    Material material, Light light, float3 halfDir,
    float NoV, float NoL, float NoH, float LoH)
{
    d2 = IsotropicLobe(material, light, halfDir, NoV, NoL, NoH, LoH);
    return IsotropicLobe(material, light, halfDir, NoV, NoL, NoH, LoH);
}

float3 DiffuseLobe(Material material, float NoV, float NoL, float LoH)
{
    d1 = Diffuse(material.roughness, NoV, NoL, LoH);
    return material.diffuseColor * Diffuse(material.roughness, NoV, NoL, LoH);
}

float3 SurfaceShading(Material material, Fragment fragment, Light light)
{
    float3 normal = fragment.normal; // Normal in ws.
    float3 lightDir = -light.direction; // Direction to light in ws.
    float3 viewDir = fragment.viewDir; // Direction to camera in ws.
    float3 halfDir = normalize(viewDir + lightDir);

    float NoV = abs(dot(normal, viewDir)) + 1e-5;
    float NoL = saturate(dot(normal, lightDir));
    float NoH = saturate(dot(normal, halfDir));
    float LoH = saturate(dot(lightDir, halfDir));

    float3 Fr = SpecularLobe(material, light, halfDir, NoV, NoL, NoH, LoH);
    float3 Fd = DiffuseLobe(material, NoV, NoL, LoH);

    float3 color = Fd + Fr /* * fragment.energyCompensation */;
    d3 = Fd + Fr;

    return color * light.color * (NoL * material.occlusion);
}

[shader("fragment")]
float4 fragmentMain(Varyings input) : SV_TARGET
{
    float3 normal = normalize(input.normal);
    float3 tangent = normalize(input.tangent);
    float2 uv = input.uv;

    normal = TangentDirToWorld(normal, tangent, GetTangentSpaceNormal(uv));

    Fragment fragment;
    fragment.position = input.worldPosition;
    fragment.normal = normalize(normal);
    fragment.viewDir = normalize(sceneData.CameraPos.xyz - input.worldPosition);

    Material material = SetupMaterial(props, uv);

    Light light = GetDirectionalLight();

    float4 color = GetColor(uv);

    float nDotL = saturate(dot(normal, -sceneData.SunlightDirection.xyz));
    //color.rgb *= nDotL * sceneData.SunlightColor.rgb;
    //color.rgb += sceneData.AmbientColor.rgb;

    float3 shading = SurfaceShading(material, fragment, light);

    // Used for debugging.
    float4 colorMap = props.ColorMap.Sample(uv);
    float3 normalMap = props.NormalMap.Sample(uv).rgb;
    float3 ormMap = props.ORMMap.Sample(uv).rgb;

    float3 outputs[6];
    outputs[0] = d1;
    outputs[1] = d2;
    outputs[2] = d3;
    outputs[3] = color.rgb + shading;
    outputs[4] = color.rgb + shading;
    outputs[5] = color.rgb + shading;

    int outputIndex = int(uv.x * 6 + uv.y + appData.Time * .1);
    //int outputIndex = 2;
    return float4(outputs[outputIndex % 6], 1);
}