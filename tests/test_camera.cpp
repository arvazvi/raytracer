#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

static int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); \
            failures++;                                                \
        }                                                              \
    } while (0)

// Runs cam.render(world) and returns what it wrote to stdout (the PPM).
static std::string render_to_string(Camera& cam, const Hittable& world) {
    std::ostringstream out;
    std::ostringstream log;
    std::streambuf* old_out = std::cout.rdbuf(out.rdbuf());
    std::streambuf* old_log = std::clog.rdbuf(log.rdbuf());
    cam.render(world);
    std::cout.rdbuf(old_out);
    std::clog.rdbuf(old_log);
    return out.str();
}

static void test_ppm_header_matches_dimensions() {
    Camera cam;
    cam.aspect_ratio = 2.0;
    cam.image_width  = 20;
    HittableList world;

    std::istringstream ppm(render_to_string(cam, world));
    std::string magic;
    int w, h, maxval;
    ppm >> magic >> w >> h >> maxval;
    CHECK(magic == "P3");
    CHECK(w == 20);
    CHECK(h == 10);
    CHECK(maxval == 255);
}

static void test_pixel_count() {
    Camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width  = 8;
    HittableList world;

    std::istringstream ppm(render_to_string(cam, world));
    std::string magic;
    int w, h, maxval;
    ppm >> magic >> w >> h >> maxval;

    int values = 0, v;
    while (ppm >> v) values++;
    CHECK(values == w * h * 3);
}

static void test_height_is_at_least_one() {
    Camera cam;
    cam.aspect_ratio = 100.0;
    cam.image_width  = 10;  // 10 / 100 -> 0, must clamp to 1
    HittableList world;

    std::istringstream ppm(render_to_string(cam, world));
    std::string magic;
    int w, h;
    ppm >> magic >> w >> h;
    CHECK(h == 1);
}

static void test_sphere_changes_center_pixel() {
    // Empty world: center pixel is sky. With a sphere in front: it must differ.
    Camera cam;
    cam.aspect_ratio = 1.0;
    cam.image_width  = 11;

    HittableList empty;
    HittableList with_sphere;
    with_sphere.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));

    std::string a = render_to_string(cam, empty);
    std::string b = render_to_string(cam, with_sphere);
    CHECK(a != b);
}

int main() {
    test_ppm_header_matches_dimensions();
    test_pixel_count();
    test_height_is_at_least_one();
    test_sphere_changes_center_pixel();

    if (failures) {
        std::printf("%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("all tests passed\n");
    return 0;
}
