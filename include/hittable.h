#pragma once

#ifndef HITTABLE_H
#define HITTABLE_H

#include "utils.h"

class Material;

class Record {
   public:
    Point3 p;
    Vector3 normal;
    double t;
    bool is_front_face;
    shared_ptr<Material> mat;

    void set_face_normal(const Ray& r, const Vector3& outward_normal) {
        is_front_face = r.Direction().dot(outward_normal) < 0;
        normal = is_front_face ? outward_normal : outward_normal * -1;
    }
};

class Hittable {
   public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval ray_t, Record& rec) const = 0;
};

#endif