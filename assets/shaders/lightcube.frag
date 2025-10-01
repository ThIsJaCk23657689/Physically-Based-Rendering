#version 450 core

out vec4 OutColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

uniform vec3 LightColor;

void main()
{
    OutColor = vec4( LightColor, 1.0f );
}