#ifndef VECTOR_H
#define VECTOR_H
#pragma once

#include <cmath>
#include <string>

#include "random.h"

class Vector3 {
    private:
        double x;
        double y;
        double z;
    public:
        Vector3();
        Vector3(double x, double y, double z);
        ~Vector3();

        double X() const;
        double Y() const;
        double Z() const;

        Vector3 operator+(const Vector3& other) const;
        Vector3 operator-(const Vector3& other) const;
        Vector3 operator*(double t) const;
        Vector3 operator/(double t) const;

        Vector3 Cross(const Vector3& other) const;
        Vector3 Unit() const;

        double Dot(const Vector3& other) const;
        double Magnitude() const;

        static Vector3 Random() {
            return Vector3(random_double(), random_double(), random_double());
        }

        static Vector3 Random(double min, double max) {
            return Vector3(random_double(min,max), random_double(min,max), random_double(min,max));
        }

};


inline Vector3 random_unit_vector() {
    while (true) {
        auto p = Vector3::Random(-1,1);
        auto lensq = p.Dot(p);
        if (1e-160 < lensq && lensq <= 1)
            return p / std::sqrt(lensq);
    }
}

inline Vector3 random_on_hemisphere(const Vector3& normal) {
    Vector3 on_unit_sphere = random_unit_vector();
    if (on_unit_sphere.Dot(normal) > 0.0)
        return on_unit_sphere;
    else
        return on_unit_sphere * -1;
}

#endif
