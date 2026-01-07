//PPM format:
// P3 (color in ASCII)
// width height
// 255 (max color value)
// r g b r g b ...

#include "main_const_util.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

using namespace std;

// determine color of pixel
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
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

    //world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

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

            color pixel_color = ray_color(r, world);
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


