#include <stdio.h>
#include "rt.h"
#include "sphere.h"
#include "camera.h"
#include "vec3.h"

// main
void wrtie_colour(vec3 colour, int samples_per_pixel){
    double scale = 1.0 / samples_per_pixel;
    printf("%d %d %d\n", (int)(256 * clamp(sqrt(colour.x * scale), 0.0, 0.999)), 
                                (int)(256 * clamp(sqrt(colour.y * scale), 0.0, 0.999)), 
                                (int)(256 * clamp(sqrt(colour.z * scale), 0.0, 0.999)));
}

vec3 hit_ray_colour(const Ray r, hit_map * world, int depth){
    //return black if it reaches max depth
    if (depth <= 0) return (vec3){0.0, 0.0, 0.0};
    hit_record rec;
    if (world_hit_object(world, r, 0.001, (double)INFINITY, &rec) == 1){
        vec3 random_vector = random_unit_vector();
        vec3 target = {rec.location.x + rec.normal.x + random_vector.x,
                        rec.location.y + rec.normal.y + random_vector.y,
                        rec.location.z + rec.normal.z + random_vector.z};
        //return (vec3){0.5 * (rec.normal.x + 1.0), 0.5 * (rec.normal.y + 1.0), 0.5 * (rec.normal.z + 1.0)};
        vec3 colour = hit_ray_colour((Ray){rec.location, (vec3){target.x - rec.location.x, target.y - rec.location.y, target.z - rec.location.z}}, world, depth - 1);
        return (vec3){0.5 * colour.x, 0.5 * colour.y, 0.5 * colour.z};
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
    camera camera = new_camera();
    int samples_per_pixel = 100;
    int max_depth = 50;

    // vec3 origin = {0, 0, 0};
    // vec3 horizontal = {viewport_width, 0, 0};
    // vec3 vertical = {0, viewport_height, 0};
    // vec3 lower_left_corner = {origin.x - horizontal.x/2 - vertical.x/2, 
    //                                 origin.y - horizontal.y/2 - vertical.y/2,
    //                                 origin.z - horizontal.z/2 - vertical.z/2 - focal_length};
    
    //Render

    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int j = image_height-1; j >= 0; --j) {
        fprintf(stderr, "\rScanlines remaining: %d , %.2f%% completed ", j, (1.0 - ((double)j / (double)image_height)) * 100);
        for (int i = 0; i < image_width; ++i) {
            // Ray r = {origin, {lower_left_corner.x + u*horizontal.x + v*vertical.x - origin.x,
            //                 lower_left_corner.y + u*horizontal.y + v*vertical.y - origin.y,
            //                 lower_left_corner.z + u*horizontal.z + v*vertical.z - origin.z}};
                
            //vec3 pixel_colour = hit_ray_colour(r, world);
            vec3 pixel_colour = {0.0, 0.0, 0.0};
            for (int s = 0; s < samples_per_pixel; ++s){
                double u = (double)(i + random_double()) / (image_width-1);
                double v = (double)(j + random_double()) / (image_height-1);
                Ray r = get_camera_ray(camera, u, v);
                pixel_colour = add(pixel_colour, hit_ray_colour(r, world, max_depth));
            }
            //vec3 pixel_colour = {((double)i / (image_width - 1)), ((double)j / (image_height -1)), 0.25};
            //fprintf(stderr, "\rinfo x: %f, y: %f, z: %f", pixel_colour.x,pixel_colour.y,pixel_colour.z);
            wrtie_colour(pixel_colour, samples_per_pixel);
        }
    }
    fprintf(stderr, "\nDone.\n");
    return 0;
}