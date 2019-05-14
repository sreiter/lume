#version 330 core

uniform vec4 solidColor = vec4 (1.f, 1.f, 0.8f, 1.f);
uniform vec4 wireColor = vec4 (0.1, 0.1, 0.1, 1.f);

in float elemLightIntensity;
in vec3 vrtDist;
in vec4 vrtColor;
out vec4 fragColor;

void main ()
{
    float minDist = 1;
    float maxDist = 2.5;
    float solidAmount = min (smoothstep (minDist, maxDist, min (vrtDist.x, vrtDist.y)),
                             min (smoothstep (minDist, maxDist, min (vrtDist.y, vrtDist.z)),
                                  smoothstep (minDist, maxDist, min (vrtDist.z, vrtDist.x))));

    // vec4 rainbowColor = 0.5 + 0.5 * vec4 (sin (150.f * vrtColor.x), cos (200.f * vrtColor.y), 1.f - sin (50.f * vrtColor.z), 1);
    // vec4 rainbowColor = 0.5f + 0.5f * sin (150.f * vrtColor);
    // vec4 color = mix (wireColor, rainbowColor, solidAmount);
    vec4 color = mix (wireColor, solidColor, solidAmount);

    fragColor = vec4 (elemLightIntensity * color.rgb, color.a);
}
