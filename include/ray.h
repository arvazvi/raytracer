#pragma once

#include "color.h"
#include "vector.h"
#ifndef RAY_H
#define RAY_H

using Point3 = Vector3;

class Ray {
   private:
    Point3 origin;
    Vector3 direction;

   public:
    Ray() {};
    Ray(const Point3& origin, const Vector3& direction);
    ~Ray();

    Point3 Origin() const;
    Vector3 Direction() const;

    Point3 At(double t) const;
};

#endif
