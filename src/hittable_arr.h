#pragma once

#include "common.h"

#include <memory>
#include <vector>

#include "flops.h"

class HittableList: public Hittable {

public:

    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    bool hit(const Ray& r, float ray_tmin, float ray_tmax, HitRecord& rec) const override {

        HitRecord temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for ( const auto& object:objects ) {

            if ( object->hit(r, ray_tmin, ray_tmax, temp_rec) ) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }

        }

        return hit_anything;

    }


private:

};
