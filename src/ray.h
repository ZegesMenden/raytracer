#pragma once

#include "vec.h"

using Point3 = Vector3;

class Ray {

private:

    Point3 orig;
    Vector3 dir;

public:

    Ray(const Point3& origin, const Vector3& direction): orig(origin), dir(direction) {}

    const Point3& origin() const { return orig; }
    const Vector3& direction() const { return dir; }

    Point3 at(float t) const {
        return orig + (dir * t);
    }

};
