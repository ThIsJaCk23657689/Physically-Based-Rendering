#version 450 core

out vec4 OutColor;

in VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

uniform vec3 LightColor;
uniform sampler2D MySampler;

void main()
{
    // OutColor = vec4( fs_in.Normal * 0.5 + 0.5, 1.0 );
    vec3 ObjectColor = texture( MySampler, fs_in.TexCoord ).xyz;
    OutColor = vec4( ObjectColor * LightColor, 1.0f );
    // OutColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}