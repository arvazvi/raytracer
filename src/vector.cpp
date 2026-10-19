#include "vector.h"

#include <cmath>

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

Vector3::~Vector3() {}

double Vector3::X() const { return x; }
double Vector3::Y() const { return y; }
double Vector3::Z() const { return z; }

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.X(), y + other.Y(), z + other.Z());
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.X(), y - other.Y(), z - other.Z());
}

Vector3 Vector3::operator*(double t) const {
    return Vector3(x * t, y * t, z * t);
}

Vector3 Vector3::operator/(double t) const {
    return Vector3(x / t, y / t, z / t);
}

Vector3 Vector3::Cross(const Vector3& other) const {
    double a = y * other.z - z * other.y;
    double b = x * other.z - z * other.x;
    double c = x * other.y - y * other.x;
    return Vector3(a, b, c);
}

Vector3 Vector3::Unit() const {
    double m = this->Magnitude();
    double a = x / m;
    double b = y / m;
    double c = z / m;

    return Vector3(a, b, c);
}

double Vector3::Dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

double Vector3::Magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}
