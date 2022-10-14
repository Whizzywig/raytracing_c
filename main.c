#include <stdio.h>
#include "rt.h"

typedef struct vec3 {
    double x;
    double y;
    double z;
} vec3;
vec3 add(vec3 a, vec3 b){
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}
vec3 mul(vec3 a, const double t){
    a.x *= t;
    a.y *= t;
    a.z *= t;
    return a;
}
vec3 div(vec3 a, const double t){
    return mul(a, 1/t);
}
double length_squared(vec3 a){
    return (a.x*a.x)+(a.y*a.y)+(a.z*a.z);
}
double length(vec3 a){
    return sqrt(length_squared(a));
}
double dot(const vec3 u, const vec3 v){
    return u.x * v.x + u.y * v.y + u.z * v.z;
}
vec3 cross(const vec3 u, const vec3 v){
    vec3 out =  {u.y * v.z - u.z * v.y,
                        u.z * v.x - u.x * v.z,
                        u.x * v.y - u.y * v.x};
    return out;
}
vec3 unit_vector(vec3 v){
    return div(v, length(v));
}

// ray
typedef struct ray {
    vec3 origin;
    vec3 direction;
} Ray;
vec3 at(Ray r, double t){
    vec3 out = {r.origin.x + t * r.direction.x,
                        r.origin.y + t * r.direction.y,
                        r.origin.z + t * r.direction.z};
    return out;
}
double hit_sphere(vec3 center, double radius, Ray r){
    vec3 oc = {r.origin.x - center.x, r.origin.y - center.y, r.origin.z - center.z};
    //double a = dot(r.direction, r.direction);
    double a = length_squared(r.direction);
    //double b = 2.0 * dot(oc, r.direction);
    double half_b = dot(oc, r.direction);
    //double c = dot(oc, oc) - radius * radius;
    double c = length_squared(oc) - (radius * radius); 
    //double discriminant = b*b - 4*a*c;
    double discriminant = half_b*half_b - a*c;
    if (discriminant < 0){
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}
vec3 ray_colour(const Ray r){
    vec3 sphere = {0,0,-1};
    double t = hit_sphere(sphere, 0.5, r);
    if (t > 0.0){
        vec3 temp = at(r, t);
        vec3 n = unit_vector((vec3){temp.x, temp.y, temp.z + 1});
        return (vec3){0.5 * (n.x+1), 0.5 * (n.y+1), 0.5 * (n.z+1)};
    }
    vec3 unit_direction = unit_vector(r.direction);
    t = 0.5*(unit_direction.y + 1.0);
    vec3 colour = {1.0-t, 1.0-t, 1.0-t};
    vec3 highlight = {t*0.5, t*0.7, t};
    return (vec3){colour.x + highlight.x, colour.y + highlight.y, colour.z + highlight.z};
}

// main
void wrtie_colour(vec3 colour){
    printf("%d %d %d\n", (int)(255.999 * colour.x), (int)(255.999 * colour.y), (int)(255.999 * colour.z));
}


int main() {
    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = (int)(image_width / aspect_ratio);

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
            vec3 pixel_colour = ray_colour(r);
            //vec3 pixel_colour = {((double)i / (image_width - 1)), ((double)j / (image_height -1)), 0.25};
            wrtie_colour(pixel_colour);
        }
    }
    fprintf(stderr, "\nDone.\n");
    return 0;
}