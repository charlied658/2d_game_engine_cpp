#type vertex
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

#type fragment
#version 330 core

in vec4 fColor;
in float fSaturation;
in vec2 fTexCoords;
in float fTexID;

uniform sampler2D tex_sampler[8];

out vec4 color;

void main() {
    int id = int(fTexID);
    if (id == 0) {
        color = fColor;
    } else {
        vec4 tex = texture(tex_sampler[id], fTexCoords);
        vec4 distance = fColor - tex;
        vec4 out_color = tex + (distance * fSaturation);
        float alpha;
        if (tex.w < 0.1f) {
            alpha = 0.0f;
        } else {
            alpha = out_color.w;
        }
        color = vec4(out_color.x, out_color.y, out_color.z, alpha);
    }
}