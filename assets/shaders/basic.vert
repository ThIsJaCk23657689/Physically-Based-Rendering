#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoord;
} vs_out;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
    vs_out.FragPos = (Model * vec4(Position.xyz, 1.0f)).xyz;
    vs_out.TexCoord = TexCoord;
    gl_Position = Projection * View * Model * vec4(Position.xyz, 1.0f);
}