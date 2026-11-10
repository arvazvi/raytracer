#pragma once

#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vector.h"
#include "interval.h"

using Color = Vector3;

static inline void write_color(std::ostream& out, const Color& pixel_color) {
    double r = pixel_color.X();
    double g = pixel_color.Y();
    double b = pixel_color.Z();

    static const Interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
