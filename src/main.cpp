#include "camera.h"
#include "cli.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "utils.h"

int main(int argc, char* argv[]) {
    Args args = Args::parse(argc, argv);
    if (args.help) {
        std::cout << Args::usage(argv[0]);
        return 0;
    }

    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2,
                          b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).magnitude() > 0.9) {
                shared_ptr<Material> sphere_material;
                if (choose_mat < 0.3) {
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Metal>(albedo);
                    world.add(
                        make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.8) {
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(
                        make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<FuzzyMetal>(albedo, fuzz);
                    world.add(
                        make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(
                        make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<FuzzyMetal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    Camera cam;

    cam.aspect_ratio = args.aspect_ratio;
    cam.image_width = args.image_width;
    cam.samples_per_pixel = args.samples_per_pixel;
    cam.max_depth = args.max_depth;

    cam.vfov = args.vfov;
    cam.lookfrom = args.lookfrom;
    cam.lookat = args.lookat;
    cam.vup = args.vup;

    cam.defocus_angle = args.defocus_angle;
    cam.focus_dist = args.focus_dist;

    cam.render(world, args.threads);
}