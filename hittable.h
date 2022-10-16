#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

typedef struct {
    vec3 location;
    vec3 normal;
    double t;
} hit_record;

typedef struct {
    int (*hit_fun)(int index, const ray r, double t_min, double t_max, hit_record rec);
    int index;
} hit_map;

int hit_object(const hit_map hm, const ray r, double t_min, double t_max, hit_record rec) {
    return hm.hit_fun(hm.index, r, t_min, t_max, rec);
}

#endif