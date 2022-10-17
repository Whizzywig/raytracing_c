#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "ray.h"

typedef struct {
    vec3 center;
    double radius;
} sphere;

static sphere SPHERES[8];
static int SPHERE_CURRENT = 0;

//TODO: figure out why it gets stuck here maybe debug with gdb
int sphere_hit(int index, const Ray r, double t_min, double t_max, hit_record rec){
    vec3 oc = {r.origin.x - SPHERES[index].center.x, r.origin.y - SPHERES[index].center.y, r.origin.z - SPHERES[index].center.z};
    double a = length_squared(r.direction);
    double half_b = dot(oc, r.direction);
    double c = length_squared(oc) - (SPHERES[index].radius * SPHERES[index].radius);
    double discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return 0;
    double sqrtd = sqrt(discriminant);

    //returns here
    double root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return 0;
    }

    //Something breaks here
    // update the record
    rec.t = root;
    rec.location = at(r, rec.t);
    vec3 outward_normal = {(rec.location.x - SPHERES[index].center.x) / SPHERES[index].radius,
                        (rec.location.y - SPHERES[index].center.y) / SPHERES[index].radius,
                        (rec.location.z - SPHERES[index].center.z) / SPHERES[index].radius};
    set_face_normal(rec, r, outward_normal);
    
    return 1;
}

hit_map create_sphere(vec3 center, double radius){
    SPHERES[SPHERE_CURRENT] = (sphere){center, radius};
    SPHERE_CURRENT++;
    hit_map out = {&sphere_hit, HIT_INDEX};
    HIT_INDEX++;
    return out;
}

#endif