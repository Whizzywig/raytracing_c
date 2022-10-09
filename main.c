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
inline double dot(const struct vec3 u, const struct vec3 v){
    return u.x * v.x + u.y * v.y + u.z * v.z;
}
inline struct vec3 cross(const struct vec3 u, const struct vec3 v){
    struct vec3 out =  {u.y * v.z - u.z * v.y,
                        u.z * v.x - u.x * v.z,
                        u.x * v.y - u.y * v.x};
    return out;
}
inline struct vec3 unit_vector(struct vec3 v){
    return div(v, length(v));
}

void wrtie_colour(struct vec3 colour){
    printf("%d %d %d\n", (int)(255.999 * colour.x), (int)(255.999 * colour.y), (int)(255.999 * colour.z));
}


int main() {
    const int image_width = 256;
    const int image_height = 256;

    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (int j = image_height-1; j >= 0; --j) {
        fprintf(stderr, "\rScanlines remaining: %d ", j);
        for (int i = 0; i < image_width; ++i) {
            struct vec3 pixel_colour = {((double)i / (image_width - 1)), ((double)j / (image_height -1)), 0.25};
            wrtie_colour(pixel_colour);
        }
    }
    fprintf(stderr, "\nDone.\n");
    return 0;
}