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
vec3 random_in_unit_sphere();
inline vec3 random_in_unit_sphere(){
    while(1){
        vec3 p = vec3_random_range(-1, 1);
        if (length_squared(p) >= 1) continue;
        return p;
    }
}
vec3 random_unit_vector();
inline vec3 random_unit_vector(){
    return unit_vector(random_in_unit_sphere());
}

vec3 random_in_hemisphere(const vec3 *normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, *normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return (vec3){-in_unit_sphere.x, -in_unit_sphere.y, -in_unit_sphere.z};
}

int near_zero(vec3 v){
    const double s = 1e-8;
    return (v.x < s) && (v.y < s) && (v.z < s);
}

vec3 reflect(const vec3 *v, const vec3 *n) {
    const double d = dot(*v, *n);
    return (vec3){v->x - 2*d*n->x, v->y - 2*d*n->y, v->z - 2*d*n->z};
}
#endif