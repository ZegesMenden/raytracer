#pragma once

#include "vec.h"
#include "flops.h"

#include <iostream>
#include <chrono>

using color = Vector3;

unsigned long long writeColor(std::ostream& stream, const color &c, int &bytes_tx) {

    char strbuf[128];

    int r = int(c.x*255.999);
    int g = int(c.y*255.999);
    int b = int(c.z*255.999);
    increment_n_flops(3);
    bytes_tx = snprintf(strbuf, 128, "%i %i %i\n", r, g, b);
    auto start = std::chrono::high_resolution_clock::now();
    // stream << r << ' ' << g << ' ' << b << '\n';
    stream.write(strbuf, bytes_tx);
    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
}
