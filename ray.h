#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// ray
typedef struct ray {
    vec3 origin;
    vec3 direction;
} Ray;
vec3 at(Ray r, double t){
    vec3 out = {r.origin.x + t * r.direction.x,
                        r.origin.y + t * r.direction.y,
                        r.origin.z + t * r.direction.z};
    return out;
}
double hit_sphere(vec3 center, double radius, Ray r){
    vec3 oc = {r.origin.x - center.x, r.origin.y - center.y, r.origin.z - center.z};
    //double a = dot(r.direction, r.direction);
    double a = length_squared(r.direction);
    //double b = 2.0 * dot(oc, r.direction);
    double half_b = dot(oc, r.direction);
    //double c = dot(oc, oc) - radius * radius;
    double c = length_squared(oc) - (radius * radius); 
    //double discriminant = b*b - 4*a*c;
    double discriminant = half_b*half_b - a*c;
    if (discriminant < 0){
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}
vec3 ray_colour(const Ray r){
    vec3 sphere = {0,0,-1};
    double t = hit_sphere(sphere, 0.5, r);
    if (t > 0.0){
        vec3 temp = at(r, t);
        vec3 n = unit_vector((vec3){temp.x, temp.y, temp.z + 1});
        return (vec3){0.5 * (n.x+1), 0.5 * (n.y+1), 0.5 * (n.z+1)};
    }
    vec3 unit_direction = unit_vector(r.direction);
    t = 0.5*(unit_direction.y + 1.0);
    vec3 colour = {1.0-t, 1.0-t, 1.0-t};
    vec3 highlight = {t*0.5, t*0.7, t};
    return (vec3){colour.x + highlight.x, colour.y + highlight.y, colour.z + highlight.z};
}

#endif