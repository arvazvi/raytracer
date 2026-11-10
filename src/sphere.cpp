#include "sphere.h"

#include <cmath>

Sphere::Sphere(const Point3& center, double radius)
    : center(center), radius(std::fmax(0, radius)) {}

bool Sphere::hit(const Ray& r, Interval ray_t,
                 Record& rec) const {
    Vector3 oc = center - r.Origin();
    const double m = r.Direction().Magnitude();
    const double a = m * m;
    const double h = r.Direction().Dot(oc);

    const double oc_m = oc.Magnitude();
    auto c = oc_m * oc_m - radius * radius;
    auto discriminant = h * h - a * c;
    if (discriminant < 0) return false;

    auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root)) return false;
    }

    rec.t = root;
    rec.p = r.At(rec.t);
    Vector3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
}