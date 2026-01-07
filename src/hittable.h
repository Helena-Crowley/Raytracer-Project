#ifndef HITTABLE_H
#define HITTABLE_H

#include "main_const_util.h"

class hit_record {
    public:
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    //outward_normal should be a unit vec
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // if ray is in same dir as normal, inside
        // if ray is in opp dir as normal, outside
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// 
class hittable {
    public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif