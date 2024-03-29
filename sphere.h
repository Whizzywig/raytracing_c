#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"
#include "ray.h"

#include <stdio.h>

struct material;

typedef struct {
    vec3 center;
    double radius;
    const struct material * mat_ptr;
} sphere;

///TODO: replace with malloc
//static sphere SPHERES[8];
static sphere *SPHERES;
static size_t SPHERES_SIZE = 0;
static int SPHERE_CURRENT = 0;

// The function handling the ray collision with a sphere
int sphere_hit(int index, const Ray r, double t_min, double t_max, hit_record *rec){
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
    rec->t = root;
    rec->location = at(r, rec->t);
    rec->mat_ptr = SPHERES[index].mat_ptr;
    //printf("\n Object hit, t=%f, x=%f, y=%f, z=%f\n",rec->t, rec->location.x, rec->location.y, rec->location.z);
    vec3 outward_normal = {(rec->location.x - SPHERES[index].center.x) / SPHERES[index].radius,
                        (rec->location.y - SPHERES[index].center.y) / SPHERES[index].radius,
                        (rec->location.z - SPHERES[index].center.z) / SPHERES[index].radius};
    //printf("\n normal, t=%f, x=%f, y=%f, z=%f\n",rec->t, outward_normal.x, outward_normal.y, outward_normal.z);
    set_face_normal(rec, r, outward_normal);
    //printf("\n face normal, x=%f, y=%f, z=%f\n", rec->normal.x, rec->normal.y, rec->normal.z);
    
    return 1;
}

void malloc_spheres(size_t size){
    SPHERES = malloc(sizeof(sphere) * size);
    SPHERES_SIZE = size;
}

///TODO: make sure there is enough space in the SPHERES var
hit_map create_sphere(vec3 center, double radius, const struct material * m){
    //if at limit grow by 50%
    if (SPHERE_CURRENT >= SPHERES_SIZE){
        realloc(SPHERES, SPHERES_SIZE + (SPHERES_SIZE/2));
        SPHERES_SIZE += SPHERES_SIZE/2;
    }
    SPHERES[SPHERE_CURRENT] = (sphere){center, radius, m};
    SPHERE_CURRENT++;
    hit_map out = {&sphere_hit, HIT_INDEX};
    HIT_INDEX++;
    return out;
}

#endif