//PPM format:
// P3 (color in ASCII)
// width height
// 255 (max color value)
// r g b r g b ...

#include "camera.h"
#include "main_const_util.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

using namespace std;

int main () {

    //world
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;

    cam.render(world);

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


