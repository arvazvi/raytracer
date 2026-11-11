#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "utils.h"

int main() {
    HittableList world;
    auto ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto lambertian = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto polished = make_shared<Metal>(Color(0.1, 0.2, 0.5));
    auto fuzzy = make_shared<FuzzyMetal>(Color(0.8, 0.6, 0.2), 1.0);
    auto glass = make_shared<Dielectric>(1.00 / 1.33);

    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, ground));
    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, lambertian));
    // world.add(make_shared<Sphere>(Point3(0.5, -0.5, -1.0), 0.1, polished));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, glass));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, fuzzy));

    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);
}
