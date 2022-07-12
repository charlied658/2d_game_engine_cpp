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