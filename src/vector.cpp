#include "vector.h"

#include <cmath>

Vector3::~Vector3() {}

double Vector3::X() const { return e[0]; }
double Vector3::Y() const { return e[1]; }
double Vector3::Z() const { return e[2]; }

Vector3 Vector3::cross(const Vector3& other) const {
    double a = e[1] * other.Z() - e[2] * other.Y();
    double b = e[2] * other.X() - e[0] * other.Z();
    double c = e[0] * other.Y() - e[1] * other.X();
    return Vector3(a, b, c);
}

Vector3 Vector3::normalize() const {
    double m = this->magnitude();
    double a = e[0] / m;
    double b = e[1] / m;
    double c = e[2] / m;

    return Vector3(a, b, c);
}

double Vector3::dot(const Vector3& other) const {
    return e[0] * other.X() + e[1] * other.Y() + e[2] * other.Z();
}

double Vector3::magnitude() const {
    return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
}
