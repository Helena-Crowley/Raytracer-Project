#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

using color = vec3;

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return sqrt(linear_component);

    return 0;
}

void write_color(ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // transform to gamma (currently in linear space)
    // aka gamma color correction
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    //pixel_color.x comes out btwn 0 and 1, translate to 255 scale
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << ' ' << '\n';
}

#endif