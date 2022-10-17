#include <stdio.h>
#include "rt.h"
#include "sphere.h"

// main
void wrtie_colour(vec3 colour){
    printf("%d %d %d\n", (int)(255.999 * colour.x), (int)(255.999 * colour.y), (int)(255.999 * colour.z));
}

vec3 hit_ray_colour(const Ray r, const hit_map world[]){
    hit_record rec;
    if (world_hit_object(world, r, 0.0, (double)INFINITY, rec) == 1){
        return (vec3){0.5 * (rec.normal.x + 1.0), 0.5 * (rec.normal.y + 1.0), 0.5 * (rec.normal.z + 1.0)};
    }
    vec3 unit_direction = unit_vector(r.direction);
    double t = 0.5*(unit_direction.y + 1.0);
    return (vec3){(1.0 - t) + (0.5 * t), (1.0 - t) + (0.7 * t), (1.0 - t) + t};
}

int main() {
    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = (int)(image_width / aspect_ratio);

    // World
    hit_map world[10];
    world[0] = create_sphere((vec3){0.0, 0.0, -1.0}, 0.5);
    world[1] = create_sphere((vec3){0.0, -100.5, -1.0}, 100.0);

    // Camera
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    vec3 origin = {0, 0, 0};
    vec3 horizontal = {viewport_width, 0, 0};
    vec3 vertical = {0, viewport_height, 0};
    vec3 lower_left_corner = {origin.x - horizontal.x/2 - vertical.x/2, 
                                    origin.y - horizontal.y/2 - vertical.y/2,
                                    origin.z - horizontal.z/2 - vertical.z/2 - focal_length};
    
    //Render

    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int j = image_height-1; j >= 0; --j) {
        fprintf(stderr, "\rScanlines remaining: %d ", j);
        for (int i = 0; i < image_width; ++i) {
            double u = (double)i / (image_width-1);
            double v = (double)j / (image_height-1);
            Ray r = {origin, {lower_left_corner.x + u*horizontal.x + v*vertical.x - origin.x,
                                    lower_left_corner.y + u*horizontal.y + v*vertical.y - origin.y,
                                    lower_left_corner.z + u*horizontal.z + v*vertical.z - origin.z}};
            vec3 pixel_colour = hit_ray_colour(r, world);
            //vec3 pixel_colour = {((double)i / (image_width - 1)), ((double)j / (image_height -1)), 0.25};
            wrtie_colour(pixel_colour);
        }
    }
    fprintf(stderr, "\nDone.\n");
    return 0;
}