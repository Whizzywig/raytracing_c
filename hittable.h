#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

#include <stdio.h>

static int HIT_INDEX = 0;

struct material;

typedef struct {
    vec3 location;
    vec3 normal;
    const struct material * mat_ptr;
    double t;
    int front_face;
} hit_record;

typedef struct {
    int (*hit_fun)(int index, const Ray r, double t_min, double t_max, hit_record *rec);
    int index;
} hit_map;

// map the ray hit to the object function
int hit_object(hit_map hm, const Ray r, double t_min, double t_max, hit_record *rec) {
    return hm.hit_fun(hm.index, r, t_min, t_max, rec);
}

// check if a ray hits any of the objects that can be hit
int world_hit_object(hit_map * world, const Ray r, double t_min, double t_max, hit_record *rec) {
    hit_record temp_rec;
    int hit = 0;
    double closest_yet = t_max;
    int temp;
    // HIT_INDEX keeps track ofd the number of items that can be hit
    for (int i = 0; i < (HIT_INDEX); i++){
        temp = hit_object(world[i], r, t_min, closest_yet, &temp_rec);
        if (temp){
            //printf("temp record normal x=%f, y=%f, z=%f\n", temp_rec.normal.x, temp_rec.normal.y, temp_rec.normal.z);
            hit = 1;
            closest_yet = temp_rec.t;
            *rec = temp_rec;
            //printf("record normal x=%f, y=%f, z=%f\n", rec->normal.x, rec->normal.y, rec->normal.z);
        }
    }
    return hit;
}

void set_face_normal(hit_record *rec, const Ray r, const vec3 outward_normal){
    rec->front_face = (dot(r.direction, outward_normal) < 0) ? 1 : 0;
    rec->normal = (rec->front_face == 1) ? outward_normal : (vec3){-outward_normal.x, -outward_normal.y, -outward_normal.z};
}

#endif