#include <cmath>
#include <cstdio>

#include "camera.h"
#include "vector.h"

// Requires X()/Y()/Z() on Vector3, and Camera::Center()/ray_direction().
static int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); \
            failures++;                                                \
        }                                                              \
    } while (0)

static bool near(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) < eps;
}

static bool eq(const Vector3& v, double x, double y, double z, double eps = 1e-9) {
    return near(v.X(), x, eps) && near(v.Y(), y, eps) && near(v.Z(), z, eps);
}

static void test_center_is_preserved() {
    Camera cam(16, 9, 1.0, Point3(1, 2, 3));
    CHECK(eq(cam.Center(), 1, 2, 3));
}

static void test_center_default_origin() {
    Camera cam(16, 9, 1.0, Point3(0, 0, 0));
    CHECK(eq(cam.Center(), 0, 0, 0));
}

static void test_ray_direction_points_into_viewport() {
    // Viewport is fixed height 2.0 and centered on the camera's -z axis.
    // The top-left pixel (0,0) should point up, left, and forward (-z).
    Camera cam(2, 2, 1.0, Point3(0, 0, 0));
    Vector3 dir = cam.ray_direction(0, 0);
    CHECK(dir.X() < 0);
    CHECK(dir.Y() > 0);
    CHECK(dir.Z() < 0);
}

static void test_ray_direction_center_pixel_is_near_axis() {
    // For an odd pixel count, the middle pixel direction should be close to
    // straight down the -z axis (small x/y offset of half a pixel).
    Camera cam(3, 3, 1.0, Point3(0, 0, 0));
    Vector3 dir = cam.ray_direction(1, 1);
    CHECK(near(dir.X(), 0, 1.0));
    CHECK(near(dir.Y(), 0, 1.0));
    CHECK(dir.Z() < 0);
}

static void test_ray_direction_varies_with_pixel() {
    Camera cam(4, 4, 1.0, Point3(0, 0, 0));
    Vector3 d0 = cam.ray_direction(0, 0);
    Vector3 d1 = cam.ray_direction(1, 0);
    CHECK(d1.X() > d0.X());
}

int main() {
    test_center_is_preserved();
    test_center_default_origin();
    test_ray_direction_points_into_viewport();
    test_ray_direction_center_pixel_is_near_axis();
    test_ray_direction_varies_with_pixel();

    if (failures) {
        std::printf("%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("all tests passed\n");
    return 0;
}
