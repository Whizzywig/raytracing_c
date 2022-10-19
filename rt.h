#ifndef RT_H
#define RT_H

#include <stdlib.h>
#include <math.h>

const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees){
    return degrees * (pi / 180.0);
}
double random_double();
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}
double random_double_range();
inline double random_double_range(double min, double max) {
    return min + ((max-min)*random_double());
}
double clamp();
inline double clamp(double x, double min, double max){
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common headers
#include "ray.h"
#include "vec3.h"
#include "hittable.h"

#endif