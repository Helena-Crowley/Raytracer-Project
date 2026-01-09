#ifndef CAMERA_H 
#define CAMERA_H

#include "hittable.h"

class camera {
    public:
    //Image
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel; // how many samples from surrounding pixels for each pixel

    void render(const hittable& world) {
        initialize();
        // rendering
        cout << "P3\n" << image_width << " " << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            clog << "\rScanlines remaining: " << (image_height - j) << ' ' << flush;
            for (int i = 0; i < image_width; i++) { // for each pixel
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world); // sum surrounding colors
                }
                write_color(cout, pixel_samples_scale * pixel_color);
            }
        }
        clog << "\rdone.                     \n";
    }

    private:
    int image_height; // rendered
    double pixel_samples_scale; // color scale factor for sum of pixel samples
    point3 center; // cam center
    point3 pixel00_loc; 
    vec3 pixel_delta_u; // pixel offset along row
    vec3 pixel_delta_v; // pixel offset along col

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height; //must be at least 1
        
        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = point3(0, 0, 0);

        // camera
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // helper vectors to iterate across viewport
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // delta vecctors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // upper left pixel location
        auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray (int i, int j) const { // return ray from origin to random pt from i,j
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i+offset.x()) * pixel_delta_u) + ((j+offset.y()) * pixel_delta_v);

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const { // return random pt in .5 range square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    color ray_color(const ray& r, const hittable& world) const {
        // determine color of pixel
        hit_record rec;
        if (world.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1, 1, 1));
        }

        // backgroud color for our vieewport
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0 - a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
    }
};

#endif