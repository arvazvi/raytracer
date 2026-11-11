#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "utils.h"

class Camera {
   public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    double vfov = 90;
    Point3 lookfrom = Point3(0, 0, 0);
    Point3 lookat = Point3(0, 0, -1);
    Vector3 vup = Vector3(0, 1, 0);

    double defocus_angle = 0;
    double focus_dist = 10;

    void render(const Hittable& world);

   private:
    int image_height;
    double pixel_samples_scale;
    Point3 center;
    Point3 pixel00_loc;
    Vector3 pixel_delta_u;
    Vector3 pixel_delta_v;

    Vector3 u, v, w;

    Vector3 defocus_disk_u;
    Vector3 defocus_disk_v;

    void initialize();
    Color ray_color(const Ray& r, int depth, const Hittable& world) const;
    Ray get_ray(int i, int j) const;
    Vector3 sample_square() const;

    Point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
};

#endif
