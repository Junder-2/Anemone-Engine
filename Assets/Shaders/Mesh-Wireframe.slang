#include "Mesh-Input.include"

// Per vertex data.
/*struct Attributes
{
    float3 position : POSITION;
    float4 color : COLOR;
};*/

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
    float4 color;
};

// Equivalent to GLSL push constants.
[[vk::push_constant]]
ConstantBuffer<PushConstants> pushConstants;


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