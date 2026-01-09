#ifndef MAIN_CONST_UTIL_H
#define MAIN_CONST_UTIL_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>

using std::make_shared;
using std::shared_ptr;
using namespace std;

// constants
const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385; // plenty good enough

// util funcs
inline double deg_to_rad(double deg) {
    return deg * pi / 180.0;
}

inline double random_double() {
    return rand() / (RAND_MAX + 1.0); // btwn 0 and 1 [0,1)
    cout << rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

// headers 
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif