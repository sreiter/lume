#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 view;
uniform mat4 projection;

out VertexShader{
    vec3 viewPos;
    vec4 vrtColor;
} vertexShader;

out vec4 vrtColor;

void main()
{
    vec4 viewPos = view * vec4 (pos, 1.0);
    vertexShader.viewPos = viewPos.xyz;
    vertexShader.vrtColor = vec4 (pos, 1.0);
    gl_Position = projection * viewPos;
}
