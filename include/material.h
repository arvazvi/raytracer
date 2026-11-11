#ifndef Material_H
#define Material_H

#pragma once

#include "hittable.h"

class Material {
   public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& r_in, const Record& rec, Color& attenuation,
                         Ray& scattered) const = 0;
};

class Lambertian : public Material {
   public:
    Lambertian(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& /*r_in*/, const Record& rec, Color& attenuation,
                 Ray& scattered) const override {
        Vector3 scatter_direction = rec.normal + random_unit_vector();
        if (scatter_direction.near_zero()) scatter_direction = rec.normal;

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

   private:
    Color albedo;
};

class Metal : public Material {
   public:
    Metal(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r_in, const Record& rec, Color& attenuation,
                 Ray& scattered) const override {
        Vector3 reflected = reflect(r_in.Direction(), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return true;
    }

   private:
    Color albedo;
};

class FuzzyMetal : public Material {
   public:
    FuzzyMetal(const Color& albedo, double fuzz)
        : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const Ray& r_in, const Record& rec, Color& attenuation,
                 Ray& scattered) const override {
        Vector3 reflected = reflect(r_in.Direction(), rec.normal);
        reflected = reflected.normalize() + (fuzz * random_unit_vector());
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return scattered.Direction().dot(rec.normal) > 0;
    }

   private:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material {
   public:
    Dielectric(double ref_idx) : ref_idx(ref_idx) {}

    bool scatter(const Ray& r_in, const Record& rec, Color& attenuation,
                 Ray& scattered) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double ri = rec.is_front_face ? (1.0 / ref_idx) : ref_idx;

        Vector3 unit_direction = r_in.Direction().normalize();
        double cos_theta = std::fmin((-unit_direction).dot(rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = ri * sin_theta > 1.0;
        Vector3 direction;
        if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, ri);
        }

        scattered = Ray(rec.p, direction);
        return true;
    }

   private:
    double ref_idx;

    static double reflectance(double cosine, double refraction_index) {
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};

#endif