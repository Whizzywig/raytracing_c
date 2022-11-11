#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt.h"

struct hit_record;


/// extra is an optional variable that is used by some materials
typedef struct material {
    vec3 albedo;
    int (*scatter_fun)(const struct material *mat, const Ray *r_in, const hit_record *rec, Ray *scattered);
    double extra;
} material;

int lambertian(const material *mat, const Ray *r_in, const hit_record *rec, Ray *scattered){
    vec3 random = random_unit_vector();
    vec3 scatter_direction = {rec->normal.x + random.x, rec->normal.y + random.y, rec->normal.z + random.z};
    //catch noise
    if (near_zero(scatter_direction)) scatter_direction = rec->normal;

    *scattered = (Ray){rec->location, scatter_direction};

    return 1;
}

// requires extra to represent the fuzzines of the metal
int metal(const material *mat, const Ray *r_in, const hit_record *rec, Ray *scattered){
    const vec3 unit = unit_vector(r_in->direction);
    vec3 reflected = reflect(&unit, &rec->normal);
    vec3 random = random_in_unit_sphere();
    reflected = (vec3){reflected.x + (mat->extra * random.x),
                        reflected.y + (mat->extra * random.y),
                        reflected.z + (mat->extra * random.z)};
    *scattered = (Ray){rec->location, reflected};

    return (dot(scattered->direction, rec->normal) > 0);
}

#endif