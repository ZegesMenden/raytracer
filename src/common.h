#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include "flops.h"

// Constants

const double inf = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double deg2rad(double degrees) {
    increment_n_flops(2);
    return degrees * pi / 180.0;
}

// Common Headers

#include "color.h"
#include "ray.h"
#include "vec.h"
