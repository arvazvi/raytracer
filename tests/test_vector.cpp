#include <cmath>
#include <cstdio>

#include "vector.h"

// Requires X()/Y()/Z() on Vector3 and `double Dot(...)`.
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

static void test_constructor() {
    Vector3 v(1, 2, 3);
    CHECK(eq(v, 1, 2, 3));
}

static void test_add() {
    CHECK(eq(Vector3(1, 2, 3) + Vector3(4, 5, 6), 5, 7, 9));
}

static void test_sub() {
    CHECK(eq(Vector3(4, 5, 6) - Vector3(1, 2, 3), 3, 3, 3));
}

static void test_scale() {
    CHECK(eq(Vector3(1, -2, 3) * 2.0f, 2, -4, 6));
}

static void test_divide() {
    CHECK(eq(Vector3(2, -4, 6) / 2.0f, 1, -2, 3));
}

static void test_dot() {
    CHECK(near(Vector3(1, 2, 3).Dot(Vector3(4, -5, 6)), 12));
    CHECK(near(Vector3(1, 0, 0).Dot(Vector3(0, 1, 0)), 0));
}

static void test_cross() {
    CHECK(eq(Vector3(1, 0, 0).Cross(Vector3(0, 1, 0)), 0, 0, 1));
    CHECK(eq(Vector3(0, 1, 0).Cross(Vector3(1, 0, 0)), 0, 0, -1));
    CHECK(eq(Vector3(1, 2, 3).Cross(Vector3(1, 2, 3)), 0, 0, 0));
}

static void test_magnitude() {
    CHECK(near(Vector3(3, 4, 0).Magnitude(), 5));
    CHECK(near(Vector3(0, 0, 0).Magnitude(), 0));
}

static void test_unit() {
    Vector3 u = Vector3(0, 3, 4).Unit();
    CHECK(eq(u, 0, 0.6, 0.8));
    CHECK(near(u.Magnitude(), 1));
}

int main() {
    test_constructor();
    test_add();
    test_sub();
    test_scale();
    test_divide();
    test_dot();
    test_cross();
    test_magnitude();
    test_unit();

    if (failures) {
        std::printf("%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("all tests passed\n");
    return 0;
}
