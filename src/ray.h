#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
    ray() {}

    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    //get origin and direction (read only)
    const point3& origin() const { return orig; }
    const vec3& direction() const { return dir; }

    //"at" is ray function
    point3 at(double t) const {
        return orig + t * dir; //P*(t) = A + t*B
    }

    private:
    point3 orig;
    vec3 dir;
};

#endif