//
// Created by Charlie Davidson on 6/27/22.
//

#include <cmath>
#include "vecmath.h"

namespace Math {
    vector2 rotate(vector2 point, vector2 origin, float radians) {
        point.x -= origin.x;
        point.y -= origin.y;

        point.x = point.x * cos(radians) - point.y * sin(radians);
        point.y = point.x * sin(radians) + point.y * cos(radians);

        point.x += origin.x;
        point.y += origin.y;
        return point;
    }
}
