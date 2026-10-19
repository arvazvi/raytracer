#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

using Point3 = Vector3;

class Camera {
    private:
        double aspect_ratio;
        double focal_length;
        double viewport_height;
        double viewport_width;
        Point3 center;

        Vector3 viewport_u;
        Vector3 viewport_v;
        Vector3 pixel_delta_u;
        Vector3 pixel_delta_v;
        Point3 viewport_upper_left;
        Point3 pixel00_loc;

        Point3 pixel_center(int i, int j) const;
    public:
        Camera(double width, double height, double focal_length, Point3 center);
        ~Camera();

        Point3 Center() const;

        Vector3 ray_direction(int i, int j) const;
};
#endif
