#include "Mesh-Input.slangh"

// This is a Debug/Example shader which outputs the world normals.

// Per vertex data.
/*struct Attributes
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 color : COLOR;
};*/

// Data passed from vertex stage to the fragment stage.
// For details on SV_Position and SV_VertexID, see the HLSL semantics:
// https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics
struct Varyings
{
    float4 position : SV_Position;
    float3 normal;
};

// Entry points should be marked using the [Shader()] attribute in Slang.
// The argument specifies which pipeline stage it is meant for.
[shader("vertex")]
Varyings vertexMain(uint vid : SV_VertexID)
{
    Varyings output;

    // Acces vertex data from the constant buffer.
    Vertex v = pushConstants.vertices[vid];

    // Transforms the local space vertex position to projection space.
    output.position = mul(pushConstants.renderMatrix, float4(v.position, 1));

    // Transforms the local space vertex normal to world space.
    // We are ignoring the 4th row and column since we only need orientation.
    output.normal = mul(float3x3(pushConstants.modelMatrix), v.normal);

    return output;
}

// Function to improve the visual of normals by remapping them to [0..1] from [-1..1].
float4 DisplayNormals(float3 normal)
{
    return float4(normal * .5 + .5, 1);
}

[shader("fragment")]
float4 fragmentMain(Varyings input) : SV_TARGET
{
    // The normals are linearly interpolated between vertices, so the mangitude will vary.
    // We normalize the normal per-fragment to make sure it has a magnitude of 1.
    float3 normal = normalize(input.normal);

    return DisplayNormals(normal);
}