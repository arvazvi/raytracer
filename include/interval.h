#pragma once

#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

class Interval {
    private:
        static constexpr double inf = std::numeric_limits<double>::infinity();

    public:
        double min;
        double max;

        Interval() : min(+inf), max(-inf) {}

        Interval(double min, double max) : min(min), max(max) {}

        double size() const {
            return max - min;
        }

        bool contains(double x) {
            return min <= x && x <= max;
        }

        bool surrounds(double x) {
            return min < x && x < max;
        }

        double clamp(double x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        static const Interval empty, universe;
};

inline const Interval Interval::empty    = Interval(+inf, -inf);
inline const Interval Interval::universe = Interval(-inf, +inf);

#endif