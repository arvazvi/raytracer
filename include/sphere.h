#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"

class Sphere : public Hittable {
   private:
    Point3 center;
    double radius;
    shared_ptr<Material> mat;

   public:
    Sphere(const Point3& center, double radius, shared_ptr<Material> mat);
    Sphere() {};
    ~Sphere() {};

    bool hit(const Ray& r, Interval ray_t, Record& rec) const override;
};

#endif