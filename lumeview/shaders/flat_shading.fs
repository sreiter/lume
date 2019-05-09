#version 330 core

uniform vec4 solidColor = vec4 (1.f, 1.f, 0.8f, 1.f);
uniform vec4 wireColor = vec4 (0.1, 0.1, 0.1, 1.f);

in float elemLightIntensity;
in vec3 vrtDist;
out vec4 fragColor;

void main ()
{
    float minDist = 1;
    float maxDist = 2.5;
    float solidAmount = min (smoothstep (minDist, maxDist, min (vrtDist.x, vrtDist.y)),
                             min (smoothstep (minDist, maxDist, min (vrtDist.y, vrtDist.z)),
                                  smoothstep (minDist, maxDist, min (vrtDist.z, vrtDist.x))));

    vec4 color = mix (wireColor, solidColor, solidAmount);

    fragColor = vec4 (elemLightIntensity * color.rgb, color.a);
}
