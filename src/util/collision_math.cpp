/**
 * @author Charlie Davidson 
 * Created on 7/8/22.
 */

#include "util/collision_math.h"

#include <cmath>

namespace Math {

    /**
     * Check if two line segments are intersecting in 1D. Used for detecting box collisions.
     * @param a_x1 Line segment 1 start
     * @param a_x2 Line segment 1 end
     * @param b_x1 Line segment 2 start
     * @param b_x2 Line segment 2 end
     * @return
     */
    bool line_segment_collision(float a_x1, float a_x2, float b_x1, float b_x2) {
        // Normalize coordinates
        if (a_x1 > a_x2) {
            float temp = a_x1;
            a_x1 = a_x2;
            a_x2 = temp;
        }
        if (b_x1 > b_x2) {
            float temp = b_x1;
            b_x1 = b_x2;
            b_x2 = temp;
        }
        // If a point from one line segment lies within the other, they are intersecting
        return (b_x1 > a_x1 && b_x1 < a_x2) ||
                (b_x2 > a_x1 && b_x2 < a_x2) ||
                (a_x1 > b_x1 && a_x1 < b_x2) ||
                (a_x2 > b_x1 && a_x2 < b_x2);
    }

    /**
     * Modulo operator that works correctly with negative numbers.
     * @param a Value
     * @param b Divisor
     * @return Modulus result
     */
    float f_mod(float a, float b) {
        if (a > 0) {
            return fmod(a,b);
        } else {
            return fmod(a,b) + b;
        }
    }
}