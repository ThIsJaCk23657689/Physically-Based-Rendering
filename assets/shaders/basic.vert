#version 450 core

layout (location = 0) in vec3 Position;

void main() {
    gl_Position = vec4(Position.xyz, 1.0f);
}