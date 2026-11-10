#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include "hittable.h"

class Camera {
    public:
        double aspect_ratio = 1.0;
        int    image_width  = 100;
        int    samples_per_pixel = 10;
        int    max_depth         = 10;

        void render(const Hittable& world);

    private:
        int     image_height;
        double  pixel_samples_scale;
        Point3  center;
        Point3  pixel00_loc;
        Vector3 pixel_delta_u;
        Vector3 pixel_delta_v;

        void initialize();
        Color ray_color(const Ray& r, int depth, const Hittable& world) const;
        Ray get_ray(int i, int j) const;
        Vector3 sample_square() const;
};

#endif
