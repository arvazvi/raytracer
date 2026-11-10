#include <iostream>

#include "utils.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#define ASPECT_RATIO (16.0 / 9.0)
#define FOCAL_LENGTH 1.0

Color ray_color(const Ray& r, const Hittable& world) {
    Record rec;
    if (world.hit(r, 0, infinity, rec))
        return (rec.normal + Color(1, 1, 1)) / 2;
    Vector3 unit_direction = r.Direction().Unit();
    double a = 0.5 * (unit_direction.Y() + 1.0);
    return Color(1.0, 1.0, 1.0) * (1.0 - a) + Color(0.5, 0.7, 1.0) * a;
}

int main() {
    int width = 800;
    int height = int(width / ASPECT_RATIO);
    height = (height < 1) ? 1 : height;
    Camera cam = Camera(width, height, FOCAL_LENGTH, Point3(0, 0, 0));

    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Render
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Ray ray = Ray(cam.Center(), cam.ray_direction(i, j));
            Color pixel_color = ray_color(ray, world);

            write_color(std::cout, pixel_color);
        }
    }
}
