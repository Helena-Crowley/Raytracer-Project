//PPM format:
// P3 (color in ASCII)
// width height
// 255 (max color value)
// r g b r g b ...

#include <iostream>
using namespace std;

int main () {
    //Image
    int image_width = 256;
    int image_height = 256;

    // rendering
    cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        clog << "\rScanlines remaining: " << (image_height - j) << ' ' << flush;
        for (int i = 0; i < image_width; i++) {
            auto r = double(i) / (image_width - 1); //
            auto g = double(j) / (image_height - 1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            cout << ir << ' ' << ig << ' ' << ib << '\n';
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


