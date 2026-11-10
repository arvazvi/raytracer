#include "sphere.h"

#include <cmath>

Sphere::Sphere(const Point3& center, double radius, shared_ptr<Material> mat)
    : center(center), radius(std::fmax(0, radius)), mat(mat) {}

bool Sphere::hit(const Ray& r, Interval ray_t, Record& rec) const {
    Vector3 oc = center - r.Origin();
    const double m = r.Direction().magnitude();
    const double a = m * m;
    const double h = r.Direction().dot(oc);

    const double oc_m = oc.magnitude();
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
    rec.mat = mat;

    return true;
}