//
// Created by Charlie Davidson on 6/27/22.
//

#pragma once

namespace Math {

    struct vec2 {
        float x, y;
    };

    struct mat4 {
        float m[16];
    };

    vec2 rotate(vec2 point, vec2 origin, float radians);

    mat4 mul(mat4 m1, mat4 m2);

    mat4 identity();

    void print_mat4(mat4 matrix);
}