#include <cmath>
#include <cstdio>

#include "ray.h"
#include "vector.h"

// Requires X()/Y()/Z() on Vector3, and Ray::Origin()/Direction()/At().
static int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); \
            failures++;                                                \
        }                                                              \
    } while (0)

static bool near(double a, double b) { return std::fabs(a - b) < 1e-9; }

static bool eq(const Vector3& v, double x, double y, double z) {
    return near(v.X(), x) && near(v.Y(), y) && near(v.Z(), z);
}

static void test_origin() {
    Ray r(Point3(1, 2, 3), Vector3(0, 0, -1));
    CHECK(eq(r.Origin(), 1, 2, 3));
}

static void test_direction() {
    Ray r(Point3(1, 2, 3), Vector3(0, 1, 0));
    CHECK(eq(r.Direction(), 0, 1, 0));
}

static void test_at_zero_returns_origin() {
    Ray r(Point3(1, 2, 3), Vector3(4, 5, 6));
    CHECK(eq(r.At(0), 1, 2, 3));
}

static void test_at_positive_t() {
    Ray r(Point3(0, 0, 0), Vector3(1, 2, 3));
    CHECK(eq(r.At(2), 2, 4, 6));
}

static void test_at_negative_t() {
    Ray r(Point3(1, 1, 1), Vector3(1, 0, 0));
    CHECK(eq(r.At(-1), 0, 1, 1));
}

int main() {
    test_origin();
    test_direction();
    test_at_zero_returns_origin();
    test_at_positive_t();
    test_at_negative_t();

    if (failures) {
        std::printf("%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("all tests passed\n");
    return 0;
}
