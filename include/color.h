#ifndef COLOR_H
#define COLOR_H

#include "vector.h"

#include <iostream>

using Color = Vector3;

static inline void write_color(std::ostream& out, const Color& pixel_color) {
    double r = pixel_color.X();
    double g = pixel_color.Y();
    double b = pixel_color.Z();

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
