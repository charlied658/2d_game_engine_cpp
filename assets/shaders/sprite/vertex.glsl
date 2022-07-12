#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in float aSaturation;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in float aTexID;

uniform mat4 view;
uniform mat4 projection;

out vec4 fColor;
out float fSaturation;
out vec2 fTexCoords;
out float fTexID;

void main() {
    fColor = aColor;
    fSaturation = aSaturation;
    fTexCoords = aTexCoords;
    fTexID = aTexID;
    gl_Position = projection * view * vec4(aPos, 0.0f, 1.0f);
}