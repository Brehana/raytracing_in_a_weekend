#ifndef COLOR_H
#define COLOR_H

#include"vec3.h"


/*
    Color Header File

    Description:    A wrapper for using vec3 to represent the color of a pixel
                    in .ppm format
*/

using color = vec3;

inline double linear_to_gamma(double linear_component){
    if(linear_component > 0){
        return sqrt(linear_component);
    }
    return 0;
}

void write_color(std::ostream& out, const color& pixel_color){
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    //Apply linear to gamma transform for gamma 2
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    //Convert x,y,z [0-1] components to the byte range [0-255]
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256.0 * intensity.clamp(r));
    int gbyte = int(256.0 * intensity.clamp(g));
    int bbyte = int(256.0 * intensity.clamp(b));

    //Write pixel out in ppm format
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif