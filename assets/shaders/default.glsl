#type vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in float aTexID;

uniform mat4 view;
uniform mat4 projection;

out vec4 fColor;
out vec2 fTexCoords;
out float fTexID;

void main() {
    fColor = aColor;
    fTexCoords = aTexCoords;
    fTexID = aTexID;
    gl_Position = projection * view * vec4(aPos, 0.0f, 1.0f);
}

#type fragment
#version 330 core

in vec4 fColor;
in vec2 fTexCoords;
in float fTexID;

uniform sampler2D tex_sampler[8];

out vec4 color;

void main() {
    int id = int(fTexID);
    if (id == 0) {
        color = fColor;
    } else {
        color = fColor * texture(tex_sampler[id], fTexCoords);
    }
}