#version 450 core

out vec4 OutColor;

in VS_OUT
{
    vec3 FragPos;
    vec2 TexCoord;
} fs_in;

uniform sampler2D MySampler;

void main()
{
    OutColor = texture( MySampler, fs_in.TexCoord );
    // OutColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}