//
// Created by Charlie Davidson on 6/27/22.
//

#include <cmath>
#include <cstdio>
#include "util/vecmath.h"

namespace Math {
    /**
     * Rotate a point about an origin by radian degrees
     * @param point Point to be rotated
     * @param origin Point of rotation
     * @param radians Radians of rotation
     * @return Rotated point
     */
    vec2 rotate(vec2 point, vec2 origin, float radians) {
        point.x -= origin.x;
        point.y -= origin.y;

        point.x = point.x * cos(radians) - point.y * sin(radians);
        point.y = point.x * sin(radians) + point.y * cos(radians);

        point.x += origin.x;
        point.y += origin.y;
        return point;
    }

    /**
     * Multiply two matrices together.
     * @param m1 Matrix 1
     * @param m2 Matrix 2
     * @return Result of multiplication
     */
    mat4 mul(mat4 m1, mat4 m2) {
        mat4 result{};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i * 4 + j] =
                        m2.m[i * 4 + 0] * m1.m[j + 0] +
                        m2.m[i * 4 + 1] * m1.m[j + 4] +
                        m2.m[i * 4 + 2] * m1.m[j + 8] +
                        m2.m[i * 4 + 3] * m1.m[j + 12];
            }
        }
        return result;
    }

    /**
     * Get the identity 4x4 matrix.
     * @return Identity
     */
    mat4 identity() {
        mat4 result{};
        for (int i = 0; i < 16; i++) {
            result.m[i] = 0;
        }
        result.m[0] = 1;
        result.m[5] = 1;
        result.m[10] = 1;
        result.m[15] = 1;
        return result;
    }

    /**
     * Print the matrix values to the screen (for debugging).
     * @param matrix Matrix to be printed
     */
    void print_mat4(mat4 matrix) {
        printf("%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n\n",
               matrix.m[0], matrix.m[4], matrix.m[8], matrix.m[12],
               matrix.m[1], matrix.m[5], matrix.m[9], matrix.m[13],
               matrix.m[2], matrix.m[6], matrix.m[10], matrix.m[14],
               matrix.m[3], matrix.m[7], matrix.m[11], matrix.m[15]);
    }
}
