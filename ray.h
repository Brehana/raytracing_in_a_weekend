#ifndef RAY_H
#define RAY_H

#include"vec3.h"

/*
    Ray Class

    Description:    All raytracing programs contain a Ray Class and  a method for computing what color is
                    seen along a line in 3D space. We think of rays as a function...
                                                        P(t) = A + tB
                    ...where A = origin of line, B = line direction, and P = a point in 3D space. This point
                    can be found using the at(double t) function in the Ray Class.
*/

class ray{
    public:
        ray() {}
        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

        const point3& origin() const { return orig; }
        const vec3& direction() const { return dir; }

        point3 at(double t) const{
            return orig + t*dir;
        }

    private:
        point3 orig;
        vec3 dir;
};

#endif