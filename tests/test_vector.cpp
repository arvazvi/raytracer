#include <cmath>
#include <cstdio>

#include "vector.h"

static int failures = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        if (!(cond)) {                                                  \
            std::printf("FAIL %s:%d  %s\n", __FILE__, __LINE__, #cond); \
            failures++;                                                 \
        }                                                               \
    } while (0)

static bool near(double a, double b) { return std::fabs(a - b) < 1e-9; }

static bool eq(const Vector3& v, double x, double y, double z) {
    return near(v.X(), x) && near(v.Y(), y) && near(v.Z(), z);
}

static void test_constructor() {
    Vector3 v(1, 2, 3);
    CHECK(eq(v, 1, 2, 3));
}

static void test_default_constructor() { CHECK(eq(Vector3(), 0, 0, 0)); }

static void test_index() {
    Vector3 v(1, 2, 3);
    CHECK(near(v[0], 1) && near(v[1], 2) && near(v[2], 3));
    v[1] = 7;
    CHECK(eq(v, 1, 7, 3));
    const Vector3 c(4, 5, 6);
    CHECK(near(c[2], 6));
}

static void test_negate() { CHECK(eq(-Vector3(1, -2, 3), -1, 2, -3)); }

static void test_compound_assign() {
    Vector3 v(1, 2, 3);
    v += Vector3(1, 1, 1);
    CHECK(eq(v, 2, 3, 4));
    v *= 2;
    CHECK(eq(v, 4, 6, 8));
    v /= 4;
    CHECK(eq(v, 1, 1.5, 2));
}

static void test_componentwise_mul() {
    CHECK(eq(Vector3(1, 2, 3) * Vector3(4, 5, 6), 4, 10, 18));
}

static void test_scalar_times_vector() {
    CHECK(eq(2.0 * Vector3(1, -2, 3), 2, -4, 6));
}

static void test_reflect() {
    Vector3 n(0, 1, 0);
    CHECK(eq(reflect(Vector3(1, -1, 0), n), 1, 1, 0));
    CHECK(eq(reflect(Vector3(0, -1, 0), n), 0, 1, 0));
    CHECK(eq(reflect(Vector3(1, 0, 0), n), 1, 0, 0));
}

static void test_almost_zero() {
    CHECK(Vector3().IsAlmostZero());
    CHECK(Vector3(1e-9, -1e-9, 0).IsAlmostZero());
    CHECK(!Vector3(0, 1e-3, 0).IsAlmostZero());
}

static void test_random() {
    for (int i = 0; i < 100; i++) {
        Vector3 r = Vector3::random(-2, 3);
        for (int j = 0; j < 3; j++) CHECK(r[j] >= -2 && r[j] < 3);
        CHECK(near(random_unit_vector().magnitude(), 1));
        CHECK(random_on_hemisphere(Vector3(0, 1, 0)).Y() >= 0);
    }
}

static void test_add() {
    CHECK(eq(Vector3(1, 2, 3) + Vector3(4, 5, 6), 5, 7, 9));
}

static void test_sub() {
    CHECK(eq(Vector3(4, 5, 6) - Vector3(1, 2, 3), 3, 3, 3));
}

static void test_scale() { CHECK(eq(Vector3(1, -2, 3) * 2.0f, 2, -4, 6)); }

static void test_divide() { CHECK(eq(Vector3(2, -4, 6) / 2.0f, 1, -2, 3)); }

static void test_dot() {
    CHECK(near(Vector3(1, 2, 3).dot(Vector3(4, -5, 6)), 12));
    CHECK(near(Vector3(1, 0, 0).dot(Vector3(0, 1, 0)), 0));
}

static void test_cross() {
    CHECK(eq(Vector3(1, 0, 0).cross(Vector3(0, 1, 0)), 0, 0, 1));
    CHECK(eq(Vector3(0, 1, 0).cross(Vector3(1, 0, 0)), 0, 0, -1));
    CHECK(eq(Vector3(1, 2, 3).cross(Vector3(1, 2, 3)), 0, 0, 0));
    CHECK(eq(Vector3(1, 0, 0).cross(Vector3(0, 0, 1)), 0, -1, 0));
    CHECK(eq(Vector3(1, 2, 3).cross(Vector3(4, 5, 6)), -3, 6, -3));
}

static void test_magnitude() {
    CHECK(near(Vector3(3, 4, 0).magnitude(), 5));
    CHECK(near(Vector3(0, 0, 0).magnitude(), 0));
}

static void test_unit() {
    Vector3 u = Vector3(0, 3, 4).normalize();
    CHECK(eq(u, 0, 0.6, 0.8));
    CHECK(near(u.magnitude(), 1));
}

int main() {
    test_constructor();
    test_default_constructor();
    test_index();
    test_negate();
    test_compound_assign();
    test_componentwise_mul();
    test_scalar_times_vector();
    test_reflect();
    test_almost_zero();
    test_random();
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
