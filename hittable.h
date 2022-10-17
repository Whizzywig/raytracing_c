#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

static int HIT_INDEX = 0;

typedef struct {
    vec3 location;
    vec3 normal;
    double t;
    int front_face;
} hit_record;

typedef struct {
    int (*hit_fun)(int index, const Ray r, double t_min, double t_max, hit_record rec);
    int index;
} hit_map;

int hit_object(const hit_map hm, const Ray r, double t_min, double t_max, hit_record rec) {
    return hm.hit_fun(hm.index, r, t_min, t_max, rec);
}

int world_hit_object(const hit_map world[], const Ray r, double t_min, double t_max, hit_record rec) {
    hit_record temp_rec;
    int hit = 0;
    double closest_yet = t_max;
    int temp;
    //sizeof(world) / sizeof(hit_map) == 10 rn
    // breaks bc only 2 items exist
    for (int i = 0; i < (2); i++){
        temp = hit_object(world[i], r, t_min, closest_yet, temp_rec);
        if (temp == 1){
            hit = 1;
            closest_yet = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit;
}

void set_face_normal(hit_record rec, const Ray r, const vec3 outward_normal){
    rec.front_face = (dot(r.direction, outward_normal) < 0) ? 1 : 0;
    rec.normal = (rec.front_face == 1) ? outward_normal : (vec3){-outward_normal.x, -outward_normal.y, -outward_normal.z}; 
}

#endif