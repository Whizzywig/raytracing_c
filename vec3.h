#ifndef VEC3_H
#define VEC3_H

#include "rt.h"
#include <math.h>

typedef struct {
    double x;
    double y;
    double z;
} vec3;
vec3 add(vec3 a, vec3 b){
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}
vec3 mul(vec3 a, const double t){
    a.x *= t;
    a.y *= t;
    a.z *= t;
    return a;
}
vec3 vec3_div(vec3 a, const double t){
    return mul(a, 1/t);
}
double length_squared(vec3 a){
    return (a.x*a.x)+(a.y*a.y)+(a.z*a.z);
}
double length(vec3 a){
    return sqrt(length_squared(a));
}
double dot(const vec3 u, const vec3 v){
    return u.x * v.x + u.y * v.y + u.z * v.z;
}
vec3 cross(const vec3 u, const vec3 v){
    vec3 out =  {u.y * v.z - u.z * v.y,
                        u.z * v.x - u.x * v.z,
                        u.x * v.y - u.y * v.x};
    return out;
}
vec3 unit_vector(vec3 v){
    return vec3_div(v, length(v));
}

inline static vec3 vec3_random(){
    return (vec3){random_double(), random_double(), random_double()};
}

inline static vec3 vec3_random_range(double min, double max){
    return (vec3){random_double_range(min, max), random_double_range(min, max), random_double_range(min, max)};
}

vec3 random_in_unit_sphere(){
    while(1){
        vec3 p = vec3_random_range(-1, 1);
        if (length_squared(p) >= 1) continue;
        return p;
    }
}
#endif