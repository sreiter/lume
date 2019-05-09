#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

uniform float ambient = 0.2;
uniform vec2 viewport = vec2 (800, 600);

in VertexShader {
    vec3 viewPos;
} vertexShader [];

out float elemLightIntensity;
out vec3 vrtDist;

float distancePointToLine (vec2 p, vec2 a, vec2 b)
{
    vec2 d = normalize (b-a);
    vec2 pProj = a + dot (p-a, d) * d;
    return distance (p, pProj);
}

void main ()
{
    vec3 v0 = (vertexShader [1].viewPos - vertexShader [0].viewPos).xyz;
    vec3 v1 = (vertexShader [2].viewPos - vertexShader [0].viewPos).xyz;

    elemLightIntensity = ambient + (1.0f - ambient) * abs(normalize(cross (v0, v1)).z);
    

    vec2 q [3];
    for (int i = 0; i < 3; ++i) {
        q [i] = viewport * gl_in[i].gl_Position.xy / gl_in[i].gl_Position.w;
    }

    vec3 distToOpposingLine = vec3 (distancePointToLine (q[0], q[1], q[2]),
                                    distancePointToLine (q[1], q[2], q[0]),
                                    distancePointToLine (q[2], q[0], q[1]));

    for (int i = 0; i < 3; ++i) {
        gl_Position = gl_in[i].gl_Position;
        vrtDist = vec3 (0, 0, 0);
        vrtDist [i] = distToOpposingLine [i];
        EmitVertex();
    }

    EndPrimitive();
}
