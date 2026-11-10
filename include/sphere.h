#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vector.h"

class Sphere : public Hittable {
   private:
    Point3 center;
    double radius;

   public:
    Sphere(const Point3& center, double radius);
    Sphere() {};
    ~Sphere() {};

    bool hit(const Ray& r, Interval ray_t,
             Record& rec) const override;
};

#endif