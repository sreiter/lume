#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 view;
uniform mat4 projection;

out VertexShader{
    vec3 viewPos;
} vertexShader;

void main()
{
    vec4 viewPos = view * vec4 (pos, 1.0);
    vertexShader.viewPos = viewPos.xyz;
    gl_Position = projection * viewPos;
}
