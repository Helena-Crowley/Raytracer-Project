//PPM format:
// P3 (color in ASCII)
// width height
// 255 (max color value)
// r g b r g b ...

#include "color.h"
#include "vec3.h"
#include "ray.h"
#include <iostream>
using namespace std;

// render sphere
double hit_sphere(const point3& center, double radius, const ray& r) {
    // math derivation in 5.1/6.2
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = h*h - a*c;

    if (discriminant < 0) {
        return -1.0;
    }
    else {
        return (h - sqrt(discriminant)) / a; // normals will be unit vecs
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    // backgroud color for our vieewport
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
}

int main () {
    //Image
    auto aspest_ratio = 16.0 / 9.0;
    int image_width = 400;

    int image_height = int(image_width / aspest_ratio);
    image_height = (image_height < 1) ? 1 : image_height; //must be at least 1

    // camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);

    // helper vectors to iterate across viewport
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // delta vecctors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // upper left pixel location
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // rendering
    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        clog << "\rScanlines remaining: " << (image_height - j) << ' ' << flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(cout, pixel_color);
        }
    }

    clog << "\rdone.                     \n";



    // Blue to green gradient
    //     for (int j = image_height - 1; j >= 0; j--) {
    //     for (int i = 0; i < image_width; i++) {
    //         auto r = 0.0; 
    //         auto g = double(i) / (image_height - 1);
    //         auto b = double(j) / (image_height - 1);

    //         int ir = int(255.999 * r);
    //         int ig = int(255.999 * g);
    //         int ib = int(255.999 * b);

    //         cout << ir << ' ' << ig << ' ' << ib << '\n';
    //     }
    // }

}


