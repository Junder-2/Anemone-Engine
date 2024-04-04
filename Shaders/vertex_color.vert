#version 450

layout (location = 0) out vec3 outColor;

void main()
{
    const vec3 positions[3] = vec3[3]
    (
        vec3( 1.0, 1.0, 0.0),
        vec3(-1.0, 1.0, 0.0),
        vec3( 0.0,-1.0, 0.0)
    );

    const vec3 colors[3] = vec3[3]
    (
        vec3(1.0, 0.0, 0.0), // Red
        vec3(0.0, 1.0, 0.0), // Green
        vec3(0.0, 0.0, 1.0)  // Blue
    );

    gl_Position = vec4(positions[gl_VertexIndex], 1.0f);
    outColor = colors[gl_VertexIndex];
}
