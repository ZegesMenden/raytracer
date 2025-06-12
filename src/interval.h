#pragma once

#include "common.h"

class Interval {
public:
    double min, max;

    Interval() : min(+inf), max(-inf) {} // Default Interval is empty

    Interval(double min, double max) : min(min), max(max) {}

    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    static const Interval empty, universe;
};

const Interval Interval::empty    = Interval(+inf, -inf);
const Interval Interval::universe = Interval(-inf, +inf);
  