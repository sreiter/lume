#version 330 core

uniform vec4 color = vec4 (1.f, 1.f, 1.f, 1.f);

in float elemLightIntensity;

out vec4 fragColor;

void main ()
{
    fragColor = vec4 (elemLightIntensity * color.rgb, color.a);
}
