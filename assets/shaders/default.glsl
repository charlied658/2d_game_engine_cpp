#type vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 view;
uniform mat4 projection;

out vec3 fColor;
out vec2 fTexCoords;

void main() {
    fColor = aColor;
    fTexCoords = aTexCoords;
    gl_Position = projection * view * vec4(aPos, 1.0);
}

#type fragment
#version 330 core

in vec3 fColor;
in vec2 fTexCoords;

uniform sampler2D tex_sampler;

out vec4 color;

void main() {
    color = texture(tex_sampler, fTexCoords);
}