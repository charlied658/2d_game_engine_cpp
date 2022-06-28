//
// Created by Charlie Davidson on 6/27/22.
//

#pragma once

struct vector2 {
    float x, y;
};

namespace Math {
    vector2 rotate(vector2 point, vector2 origin, float radians);
}