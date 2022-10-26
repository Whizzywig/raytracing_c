#ifndef MATERIAL_H
#define MATERIAL_H

#include "rt.h"

struct hit_record;

typedef struct material {
    vec3 albedo;
    int (*scatter_fun)(const Ray *r_in, const hit_record *rec, Ray *scattered);
} material;

int lambertian(const Ray *r_in, const hit_record *rec, Ray *scattered){
    vec3 random = random_unit_vector();
    vec3 scatter_direction = {rec->normal.x + random.x, rec->normal.y + random.y, rec->normal.z + random.z};
    //catch noise
    if (near_zero(scatter_direction)) scatter_direction = rec->normal;

    *scattered = (Ray){rec->location, scatter_direction};

    return 1;
}

int metal(const Ray *r_in, const hit_record *rec, Ray *scattered){
    const vec3 unit = unit_vector(r_in->direction);
    vec3 reflected = reflect(&unit, &rec->normal);
    *scattered = (Ray){rec->location, reflected};

    return (dot(scattered->direction, rec->normal) > 0);
}
#endif