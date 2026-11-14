#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "cli.h"

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

// Builds a mutable argv (with a program name at argv[0]) and parses it.
static Args parse(std::vector<std::string> args) {
    args.insert(args.begin(), "raytracer");
    std::vector<char*> argv;
    for (auto& a : args) argv.push_back(&a[0]);
    argv.push_back(nullptr);
    return Args::parse(static_cast<int>(args.size()), argv.data());
}

template <typename F>
static bool throws_invalid_argument(F f) {
    try {
        f();
    } catch (const std::invalid_argument&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

static void test_defaults() {
    Args a = parse({});
    CHECK(near(a.aspect_ratio, 16.0 / 9.0));
    CHECK(a.image_width == 1200);
    CHECK(a.samples_per_pixel == 500);
    CHECK(a.max_depth == 50);
    CHECK(a.vfov == 20);
    CHECK(eq(a.lookfrom, 13, 2, 3));
    CHECK(eq(a.lookat, 0, 0, 0));
    CHECK(eq(a.vup, 0, 1, 0));
    CHECK(near(a.defocus_angle, 0.6));
    CHECK(near(a.focus_dist, 10.0));
}

static void test_width() {
    CHECK(parse({"-w", "800"}).image_width == 800);
    CHECK(parse({"--width", "640"}).image_width == 640);
}

static void test_samples_per_pixel() {
    CHECK(parse({"-s", "10"}).samples_per_pixel == 10);
    CHECK(parse({"--samples-pp", "42"}).samples_per_pixel == 42);
}

static void test_max_depth() { CHECK(parse({"--max-depth", "7"}).max_depth == 7); }

static void test_vfov() { CHECK(parse({"--vfov", "90"}).vfov == 90); }

static void test_doubles() {
    CHECK(near(parse({"--defocus-angle", "1.25"}).defocus_angle, 1.25));
    CHECK(near(parse({"--aspect-ratio", "1.5"}).aspect_ratio, 1.5));
    CHECK(near(parse({"--focus-dist", "3.4"}).focus_dist, 3.4));
}

static void test_vectors() {
    CHECK(eq(parse({"--look-from", "1,2,3"}).lookfrom, 1, 2, 3));
    CHECK(eq(parse({"--look-at", "-1.5,0,2.25"}).lookat, -1.5, 0, 2.25));
    CHECK(eq(parse({"--vup", "0,0,1"}).vup, 0, 0, 1));
}

static void test_multiple_flags() {
    Args a = parse({"-w", "400", "-s", "8", "--max-depth", "5", "--vfov",
                    "45", "--look-from", "0,1,2", "--focus-dist", "2.5"});
    CHECK(a.image_width == 400);
    CHECK(a.samples_per_pixel == 8);
    CHECK(a.max_depth == 5);
    CHECK(a.vfov == 45);
    CHECK(eq(a.lookfrom, 0, 1, 2));
    CHECK(near(a.focus_dist, 2.5));
    // Untouched fields keep their defaults.
    CHECK(eq(a.lookat, 0, 0, 0));
    CHECK(near(a.defocus_angle, 0.6));
}

static void test_last_flag_wins() {
    CHECK(parse({"-w", "100", "--width", "200"}).image_width == 200);
}

static void test_unknown_args_ignored() {
    Args a = parse({"--bogus", "foo", "-w", "320", "extra"});
    CHECK(a.image_width == 320);
    CHECK(a.samples_per_pixel == 500);
}

static void test_invalid_number_throws() {
    CHECK(throws_invalid_argument([] { parse({"-w", "abc"}); }));
    CHECK(throws_invalid_argument([] { parse({"--focus-dist", "x"}); }));
}

static void test_invalid_vector_throws() {
    CHECK(throws_invalid_argument([] { parse({"--look-from", "1,2"}); }));
    CHECK(throws_invalid_argument([] { parse({"--look-at", "1,2,3,4"}); }));
    CHECK(throws_invalid_argument([] { parse({"--vup", "a,b,c"}); }));
}

template <typename F>
static bool throws_out_of_range(F f) {
    try {
        f();
    } catch (const std::out_of_range&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

static void test_threads_initialized() {
    Args a = parse({});
    CHECK(a.threads == 1);
}

static void test_threads_flag() {
    CHECK(parse({"-j", "1"}).threads == 1);
    CHECK(parse({"--threads", "1"}).threads == 1);
    unsigned int hw = std::max(1u, std::thread::hardware_concurrency());
    CHECK(parse({"-j", std::to_string(hw)}).threads == hw);
    CHECK(throws_invalid_argument([] { parse({"-j", "0"}); }));
    CHECK(throws_invalid_argument([] { parse({"-j", "-2"}); }));
    CHECK(throws_out_of_range([&] { parse({"-j", std::to_string(hw + 1)}); }));
}

static void test_missing_value_throws() {
    const char* flags[] = {"-w",           "--width",        "-s",
                           "--samples-pp", "--max-depth",    "--vfov",
                           "--defocus-angle", "--aspect-ratio", "--focus-dist",
                           "--look-from",  "--look-at",      "--vup"};
    for (const char* f : flags) {
        std::string flag = f;
        bool threw = throws_invalid_argument([&] { parse({flag}); });
        if (!threw) std::printf("  no throw for trailing %s\n", f);
        CHECK(threw);
    }
    // A missing value after earlier valid flags is still detected.
    CHECK(throws_invalid_argument([] { parse({"-w", "100", "-s"}); }));
}

static void test_negative_uint_throws() {
    CHECK(throws_invalid_argument([] { parse({"-w", "-5"}); }));
    CHECK(throws_invalid_argument([] { parse({"--samples-pp", "-1"}); }));
    CHECK(throws_invalid_argument([] { parse({"--max-depth", "-10"}); }));
    CHECK(throws_invalid_argument([] { parse({"--vfov", "-90"}); }));
}

static void test_uint_too_large_throws() {
    CHECK(throws_out_of_range([] { parse({"-w", "4294967296"}); }));
    CHECK(parse({"-w", "4294967295"}).image_width == 4294967295u);
    CHECK(parse({"-w", "0"}).image_width == 0);
}

static bool contains(const std::string& s, const std::string& sub) {
    return s.find(sub) != std::string::npos;
}

static void test_help() {
    CHECK(!parse({}).help);
    CHECK(parse({"-h"}).help);
    CHECK(parse({"--help"}).help);
    CHECK(parse({"-w", "100", "--help"}).help);
}

static void test_help_stops_parsing() {
    // Anything after --help is ignored, even input that would otherwise throw.
    Args a = parse({"--help", "-w", "abc"});
    CHECK(a.help);
    CHECK(a.image_width == 1200);
    CHECK(parse({"-h", "-w"}).help);
}

static void test_usage_lists_options() {
    std::string u = Args::usage("raytracer");
    CHECK(contains(u, "Usage: raytracer"));
    const char* opts[] = {"-h, --help",      "-w, --width",   "-s, --samples-pp",
                          "--max-depth",     "--aspect-ratio", "--vfov",
                          "--look-from",     "--look-at",     "--vup",
                          "--defocus-angle", "--focus-dist"};
    for (const char* o : opts) {
        bool found = contains(u, o);
        if (!found) std::printf("  usage missing %s\n", o);
        CHECK(found);
    }
}

static void test_usage_shows_defaults() {
    std::string u = Args::usage("raytracer");
    CHECK(contains(u, "(default 1200)"));
    CHECK(contains(u, "(default 500)"));
    CHECK(contains(u, "(default 13,2,3)"));
    CHECK(contains(u, "(default 0,1,0)"));
}

int main() {
    test_defaults();
    test_width();
    test_samples_per_pixel();
    test_max_depth();
    test_vfov();
    test_doubles();
    test_vectors();
    test_multiple_flags();
    test_last_flag_wins();
    test_unknown_args_ignored();
    test_invalid_number_throws();
    test_invalid_vector_throws();
    test_threads_initialized();
    test_threads_flag();
    test_missing_value_throws();
    test_negative_uint_throws();
    test_uint_too_large_throws();
    test_help();
    test_help_stops_parsing();
    test_usage_lists_options();
    test_usage_shows_defaults();

    if (failures) {
        std::printf("%d check(s) failed\n", failures);
        return 1;
    }
    std::printf("all tests passed\n");
    return 0;
}
