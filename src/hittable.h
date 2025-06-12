#pragma once

#include "common.h"

#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "flops.h"

class HitRecord {

public:
    Point3 p;
    Vector3 normal;
    float t;
    bool front_face;

    void setNormalFace(const Ray &r, const Vector3 &outwardNormal) {

        front_face = r.direction().dot(outwardNormal) < 0;
        normal = front_face ? outwardNormal : -outwardNormal;

    }
};

class Hittable {

public:

    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, float ray_tmin, float ray_tmax, HitRecord& rec) const = 0;

};

class Sphere: public Hittable {

private:

    Point3 center;
    float radius;

public:

    Sphere(const Point3& center, float radius): center(center), radius(fmaxf(0, radius)) {}

    bool hit (const Ray& r, float ray_tmin, float ray_tmax, HitRecord& rec) const override {

        Vector3 oc = center - r.origin();
        auto a = r.direction().len(); a *= a;
        auto h = r.direction().dot(oc);
        auto c = oc.len(); c *= c;
        c -= radius*radius;
        auto discriminant = (h*h) - (a*c);
        increment_n_flops(6);
        if ( discriminant < 0 ) {
            return false;
        } 

        float sqrtd = sqrtf(discriminant);

        float root = (h - sqrtd) / a;
        increment_n_flops(3);
        if (root <= ray_tmin || ray_tmax <= root) {
            root = (h + sqrtd) / a;
            increment_n_flops(2);
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);

        Vector3 outwardNormal = (rec.p - center) / radius;
        rec.setNormalFace(r, outwardNormal);

        return true;
    }

};