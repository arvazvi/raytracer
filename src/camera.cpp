#include "camera.h"
#include "vector.h"

Camera::Camera(double width, double height, double focal_length, Point3 center) {
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(width)/height);

    this->focal_length = focal_length;
    this->viewport_height = viewport_height;
    this->viewport_width = viewport_width;
    this->center = center;

    this->viewport_u = Vector3(viewport_width, 0, 0);
    this->viewport_v = Vector3(0, -viewport_height, 0);

    this->pixel_delta_u = viewport_u / width;
    this->pixel_delta_v = viewport_v / height;

    this->viewport_upper_left = center - Vector3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    this->pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) / 2;
}

Camera::~Camera(){}


Point3 Camera::Center() const {
    return center;
}

Vector3 Camera::ray_direction(int i, int j) const {
    Point3 pixel_center = pixel00_loc + (pixel_delta_u * i) + (pixel_delta_v * j);
    return pixel_center - center;
}
