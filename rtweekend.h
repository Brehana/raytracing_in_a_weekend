#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include<cmath>
#include<cstdlib>
#include<iostream>
#include<limits>
#include<memory>

//Common utilities and constants for my weekend raytracer

//C++ STD usings

using std::fabs;
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

//Constants

const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

//Utility functions

inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180;
}

//Returns a random real number in [0,1).
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

//Returns a random real number in [min,max).
inline double random_double(double min, double max) {
    return min + (max-min) * random_double();
}


//Common headers

#include"interval.h"
#include"color.h"
#include"ray.h"
#include"vec3.h"


#endif