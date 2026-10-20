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

    bool hit(const Ray& r, double ray_tmin, double ray_tmax,
             Record& rec) const override;
};

#endif