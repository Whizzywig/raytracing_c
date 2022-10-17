#ifndef CAMERA_H
#define CAMERA_H

#include "rt.h"

typedef struct {
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
} camera;

camera new_camera(){
    double aspect_ratio = 16.0 / 9.0;
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    vec3 origin = {0.0, 0.0, 0.0};
    vec3 horizontal = {viewport_width, 0.0, 0.0};
    vec3 vertical = {0.0, viewport_height, 0.0};
    vec3 lower_left_corner = {origin.x - (horizontal.x/2) - (vertical.x/2),
                            origin.y - (horizontal.y/2) - (vertical.y/2),
                            origin.z - (horizontal.z/2) - (vertical.z/2) - focal_length};

    return (camera){origin, lower_left_corner, horizontal, vertical};
}

Ray get_camera_ray(const camera c, double u, double v){
    return (Ray){c.origin, (vec3){c.lower_left_corner.x + (u * c.horizontal.x) + (v * c.vertical.x) - c.origin.x,
                                c.lower_left_corner.y + (u * c.horizontal.y) + (v * c.vertical.y) - c.origin.y,
                                c.lower_left_corner.z + (u * c.horizontal.z) + (v * c.vertical.z) - c.origin.z}};
}

#endif