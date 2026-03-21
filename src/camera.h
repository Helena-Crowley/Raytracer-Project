#ifndef CAMERA_H 
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
    public:
    //Image
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10; // how many samples from surrounding pixels for each pixel
    int max_depth = 10; // max number of ray bounces allowed
    
    double vfov = 90; // field of view aka vertical view angle
    point3 lookfrom = point3(0,0,0);   // point camera is looking from
    point3 lookat   = point3(0,0,-1);  // point camera is looking at
    vec3   vup      = vec3(0,1,0);     // up relative to cam

    double defocus_angle = 0; // cone angle
    double focus_dist = 10; // distance from cam to in-focus plane

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
                    pixel_color += ray_color(r, max_depth, world); // sum surrounding colors
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
    vec3 u, v, w; // cam frame basis vectors
    vec3 defocus_disk_u; // disk horizontal radius
    vec3 defocus_disk_v; // disk vertical radius

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height; //must be at least 1
        
        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // camera
        auto theta = deg_to_rad(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // helper vectors to iterate across viewport
        vec3 viewport_u = viewport_width * u; 
        vec3 viewport_v = viewport_height * -v; 

        // delta vecctors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // upper left pixel location
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // camera defocus disk basis vectors
        auto defocus_radius = focus_dist * tan(deg_to_rad(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray (int i, int j) const { // return ray from origin on defocus disk to random pt from i,j
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc + ((i+offset.x()) * pixel_delta_u) + ((j+offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const { // return random pt in .5 range square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const { // return random point in defocus disk
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

    color ray_color(const ray& r, int depth, const hittable& world) const { // determine color of pixel
        // no more light returned after hitting ray bounce limit
        if (depth <= 0)
            return color(0,0,0);

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
            // vec3 direction = rec.normal + random_unit_vector(); // lambertian diffuse model (rays more likely to scatter close to normal)
            // //vec3 direction = random_on_hemisphere(rec.normal); // simple random diffuse model
            // return 0.5 * ray_color(ray(rec.p, direction), depth-1, world); // 0.5 = bounced ray keeps 50% of its color, meaning grey
        }

        // backgroud color for our vieewport
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5*(unit_direction.y() + 1.0);
        return (1.0 - a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
    }
};

#endif