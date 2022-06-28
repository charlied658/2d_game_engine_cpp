//
// Created by Charlie Davidson on 6/27/22.
//

#include <cmath>
#include "util/vecmath.h"

namespace Math {
    /**
     * Rotate a point about an origin by radian degrees
     * @param point Point to be rotated
     * @param origin Point of rotation
     * @param radians Radians of rotation
     * @return Rotated point
     */
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
