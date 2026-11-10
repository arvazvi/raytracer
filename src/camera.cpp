#include "camera.h"

void Camera::render(const Hittable& world) {
    initialize();

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            Color pixel_color(0,0,0);
            for (int sample = 0; sample < samples_per_pixel; sample++) {
                Ray r = get_ray(i, j);
                pixel_color = pixel_color + ray_color(r, world);
            }
            write_color(std::cout, pixel_color * pixel_samples_scale);
        }
    }

    std::clog << "\rDone.                 \n";
}

void Camera::initialize() {
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    center = Point3(0, 0, 0);

    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(image_width) / image_height);

    Vector3 viewport_u = Vector3(viewport_width, 0, 0);
    Vector3 viewport_v = Vector3(0, -viewport_height, 0);

    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    Point3 viewport_upper_left =
        center - Vector3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;
}

Color Camera::ray_color(const Ray& r, const Hittable& world) const {
    Record rec;

    if (world.hit(r, Interval(0, infinity), rec)) {
        Vector3 direction = random_on_hemisphere(rec.normal);
        return ray_color(Ray(rec.p, direction), world) * 0.5;
    }

    Vector3 unit_direction = r.Direction().Unit();
    double a = 0.5 * (unit_direction.Y() + 1.0);
    return Color(1.0, 1.0, 1.0) * (1.0-a) + Color(0.5, 0.7, 1.0) * a;
}

Ray Camera::get_ray(int i, int j) const {
    auto offset = sample_square();
    auto pixel_sample = pixel00_loc
        + (pixel_delta_u * (i + offset.X()))
        + (pixel_delta_v * (j + offset.Y()));

    auto ray_origin = center;
    auto ray_direction = pixel_sample - ray_origin;

    return Ray(ray_origin, ray_direction);
}

Vector3 Camera::sample_square() const {
    return Vector3(random_double() - 0.5, random_double() - 0.5, 0);
}