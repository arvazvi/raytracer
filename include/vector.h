#ifndef VECTOR_H
#define VECTOR_H
#pragma once

#include <cmath>
#include <iostream>
#include <string>

#include "random.h"

class Vector3 {
   public:
    double e[3];
    Vector3() : e{0, 0, 0} {};
    Vector3(double x, double y, double z) : e{x, y, z} {};
    ~Vector3();

    double X() const;
    double Y() const;
    double Z() const;

    Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    Vector3& operator+=(const Vector3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vector3& operator/=(double t) { return *this *= 1 / t; }

    Vector3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3 cross(const Vector3& other) const;
    Vector3 normalize() const;

    double dot(const Vector3& other) const;
    double magnitude() const;

    bool IsAlmostZero() const {
        const double s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) &&
               (std::fabs(e[2]) < s);
    }

    static Vector3 random() {
        return Vector3(random_double(), random_double(), random_double());
    }

    static Vector3 random(double min, double max) {
        return Vector3(random_double(min, max), random_double(min, max),
                       random_double(min, max));
    }
};

inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v) {
    return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v) {
    return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v) {
    return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(double t, const Vector3& v) {
    return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vector3 operator*(const Vector3& v, double t) { return t * v; }

inline Vector3 operator/(const Vector3& v, double t) { return (1 / t) * v; }

inline Vector3 reflect(const Vector3& v, const Vector3& n) {
    return v - 2 * v.dot(n) * n;
}

inline Vector3 random_unit_vector() {
    while (true) {
        auto p = Vector3::random(-1, 1);
        auto lensq = p.dot(p);
        if (1e-160 < lensq && lensq <= 1) return p / std::sqrt(lensq);
    }
}

inline Vector3 random_on_hemisphere(const Vector3& normal) {
    Vector3 on_unit_sphere = random_unit_vector();
    if (on_unit_sphere.dot(normal) > 0.0)
        return on_unit_sphere;
    else
        return on_unit_sphere * -1;
}

inline Vector3 refract(const Vector3& uv, const Vector3& n,
                       double etai_ov_etat) {
    double cos_theta = std::fmin((-uv).dot(n), 1.0);
    Vector3 r_out_perp = etai_ov_etat * (uv + cos_theta * n);
    Vector3 r_out_parallel =
        -std::sqrt(std::fabs(1.0 - r_out_perp.dot(r_out_perp))) * n;
    return r_out_perp + r_out_parallel;
}

#endif
