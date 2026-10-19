#include "camera.h"
#include "color.h"
#include "ray.h"
#include "vector.h"

#include <iostream>

#define ASPECT_RATIO  (16.0 / 9.0)
#define FOCAL_LENGTH 1.0

double hit_sphere(const Point3& center, double radius, const Ray& r) {
    Vector3 oc = center - r.Origin();
    auto a = r.Direction().Dot(r.Direction());
    auto b = -2.0 * r.Direction().Dot(oc);
    auto c = oc.Dot(oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) return -1.0;

    return (-b - std::sqrt(discriminant) ) / (2.0*a);
}

Color ray_color(const Ray& r) {
    auto t = hit_sphere(Point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        Vector3 N = (r.At(t) - Vector3(0,0,-1)).Unit();
        return Color(N.X()+1, N.Y()+1, N.Z()+1) / 2;
    }
    Vector3 unit_direction = r.Direction().Unit();
    auto a = 0.5*(unit_direction.Y() + 1.0);
    return Color(1.0, 1.0, 1.0)*(1.0-a) + Color(0.5, 0.7, 1.0) * a;
}

int main() {
    int width = 800;
    int height = int(width / ASPECT_RATIO);
    height = (height < 1) ? 1 : height;
    Camera cam = Camera(width, height, FOCAL_LENGTH, Point3(0, 0, 0));

    // Render
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Ray ray = Ray(cam.Center(), cam.ray_direction(i, j));
            Color pixel_color = ray_color(ray);

            write_color(std::cout, pixel_color);
        }
    }

}
