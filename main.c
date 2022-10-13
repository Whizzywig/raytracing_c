#include <stdio.h>
#include <math.h>

struct vec3 {
    double x;
    double y;
    double z;
};
struct vec3 add(struct vec3 a, struct vec3 b){
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}
struct vec3 mul(struct vec3 a, const double t){
    a.x *= t;
    a.y *= t;
    a.z *= t;
    return a;
}
struct vec3 div(struct vec3 a, const double t){
    return mul(a, 1/t);
}
double length_squared(struct vec3 a){
    return (a.x*a.x)+(a.y*a.y)+(a.z*a.z);
}
double length(struct vec3 a){
    return sqrt(length_squared(a));
}
double dot(const struct vec3 u, const struct vec3 v){
    return u.x * v.x + u.y * v.y + u.z * v.z;
}
struct vec3 cross(const struct vec3 u, const struct vec3 v){
    struct vec3 out =  {u.y * v.z - u.z * v.y,
                        u.z * v.x - u.x * v.z,
                        u.x * v.y - u.y * v.x};
    return out;
}
struct vec3 unit_vector(struct vec3 v){
    return div(v, length(v));
}

// ray
struct ray {
    struct vec3 origin;
    struct vec3 direction;
};
struct vec3 at(struct ray r, double t){
    struct vec3 out = {r.origin.x + t * r.direction.x,
                        r.origin.y + t * r.direction.y,
                        r.origin.z + t * r.direction.z};
    return out;
}
double hit_sphere(struct vec3 center, double radius, struct ray r){
    struct vec3 oc = {r.origin.x - center.x, r.origin.y - center.y, r.origin.z - center.z};
    double a = dot(r.direction, r.direction);
    double b = 2.0 * dot(oc, r.direction);
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b*b - 4*a*c;
    if (discriminant < 0){
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}
struct vec3 ray_colour(const struct ray r){
    struct vec3 sphere = {0,0,-1};
    double t = hit_sphere(sphere, 0.5, r);
    if (t > 0.0){
        struct vec3 temp = at(r, t);
        struct vec3 n = unit_vector((struct vec3){temp.x, temp.y, temp.z + 1});
        return (struct vec3){0.5 * (n.x+1), 0.5 * (n.y+1), 0.5 * (n.z+1)};
    }
    struct vec3 unit_direction = unit_vector(r.direction);
    t = 0.5*(unit_direction.y + 1.0);
    struct vec3 colour = {1.0-t, 1.0-t, 1.0-t};
    struct vec3 highlight = {t*0.5, t*0.7, t};
    return (struct vec3){colour.x + highlight.x, colour.y + highlight.y, colour.z + highlight.z};
}

// main
void wrtie_colour(struct vec3 colour){
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

    struct vec3 origin = {0, 0, 0};
    struct vec3 horizontal = {viewport_width, 0, 0};
    struct vec3 vertical = {0, viewport_height, 0};
    struct vec3 lower_left_corner = {origin.x - horizontal.x/2 - vertical.x/2, 
                                    origin.y - horizontal.y/2 - vertical.y/2,
                                    origin.z - horizontal.z/2 - vertical.z/2 - focal_length};
    
    //Render

    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int j = image_height-1; j >= 0; --j) {
        fprintf(stderr, "\rScanlines remaining: %d ", j);
        for (int i = 0; i < image_width; ++i) {
            double u = (double)i / (image_width-1);
            double v = (double)j / (image_height-1);
            struct ray r = {origin, {lower_left_corner.x + u*horizontal.x + v*vertical.x - origin.x,
                                    lower_left_corner.y + u*horizontal.y + v*vertical.y - origin.y,
                                    lower_left_corner.z + u*horizontal.z + v*vertical.z - origin.z}};
            struct vec3 pixel_colour = ray_colour(r);
            //struct vec3 pixel_colour = {((double)i / (image_width - 1)), ((double)j / (image_height -1)), 0.25};
            wrtie_colour(pixel_colour);
        }
    }
    fprintf(stderr, "\nDone.\n");
    return 0;
}