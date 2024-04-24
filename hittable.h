#ifndef HITTABLE_H
#define HITTABLE_H

class material;

//ADT for a bunch of things important to know about a ray
//collision.
class hit_record {
    public:
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        double t;
        bool front_facing;

        //Sets the hit record normal vector.
        //NOTE: the parameter 'outward_normal' is assumed to have unit length.
        void set_face_normal(const ray& r, const vec3& outward_normal) {
            front_facing = dot(r.direction(), outward_normal) < 0;
            normal = front_facing ? outward_normal : -outward_normal;
        }
};

//Abstract Class for creating polygons or other renderables in the raytracer.
class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif