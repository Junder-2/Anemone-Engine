#include "Mesh-Input.slangh"

// Per vertex data.
/*struct Attributes
{
    float3 position : POSITION;
    float4 color : COLOR;
};*/

// Data passed from vertex to the next stage.
struct Varyings
{
    float4 position : SV_Position;
    float4 color;
};

[shader("vertex")]
Varyings vertexMain(uint vid : SV_VertexID)
{
    Varyings output;

    Vertex v = pushConstants.vertices[vid];
    output.position = mul(pushConstants.renderMatrix, float4(v.position, 1));
    output.color = v.color;

    return output;
}

[shader("fragment")]
float4 fragmentMain(Varyings input) : SV_TARGET
{
    return input.color;
}