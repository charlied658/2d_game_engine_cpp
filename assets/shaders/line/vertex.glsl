#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 view;
uniform mat4 projection;

out vec4 fColor;

void main() {
    fColor = aColor;
    gl_Position = projection * view * vec4(aPos, 0.0f, 1.0f);
}