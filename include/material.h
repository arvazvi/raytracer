#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once

#include "ray.h"

class Material {
   public:
    virtual ~Material() = default;

    virtual Ray& reflect(const Ray& r) const = 0;
};

#endif