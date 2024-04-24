#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include"hittable.h"

#include<vector> //contains "vector" collection


class hittable_list : public hittable {
    public:
        std::vector<shared_ptr<hittable>> objects; //A vector collection of shared_ptrs to hittable objects.

        hittable_list() {}
        hittable_list(shared_ptr<hittable> object)  { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hittable> object) {
            objects.push_back(object);
        }
        //Determines whether a given ray hits any hittable object in the list.
        //Fills hit_record with details of nearest hit.
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            hit_record temp_rec;
            bool hit_anything = false;
            double closest_so_far = ray_t.max;

            for(const auto& object : objects) { //For object in objects...
                if(object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {    //If a ray hits an object...
                    hit_anything = true;
                    closest_so_far = temp_rec.t;
                    rec = temp_rec;
                }
            }

            return hit_anything;
        }
};

#endif