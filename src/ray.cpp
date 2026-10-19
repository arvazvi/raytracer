#include "ray.h"
#include "vector.h"

Ray::Ray(const Point3& origin, const Vector3& direction) : origin(origin), direction(direction) {};
Ray::~Ray() {};

Point3 Ray::Origin() const {
    return origin;
}

Vector3 Ray::Direction() const {
    return direction;
}

Point3 Ray::At(double t) const {
    return origin + (direction * t);
}
