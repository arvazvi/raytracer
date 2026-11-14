#pragma once
#ifndef CLI_H
#define CLI_H

#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "ray.h"
#include "string.h"
#include "vector.h"

class Args {
   public:
    unsigned int threads;

    double aspect_ratio = 16.0 / 9.0;
    unsigned int image_width = 1200;
    unsigned int samples_per_pixel = 500;
    unsigned int max_depth = 50;

    unsigned int vfov = 20;
    Point3 lookfrom = Point3(13, 2, 3);
    Point3 lookat = Point3(0, 0, 0);
    Vector3 vup = Vector3(0, 1, 0);

    double defocus_angle = 0.6;
    double focus_dist = 10.0;

    bool help = false;

    Args() {
        unsigned int hw = std::thread::hardware_concurrency();
        threads = hw > 0 ? hw : 1;
    };

    static Args parse(int argc, char* argv[]) {
        Args args;
        std::vector<std::string> arr(argv + 1, argv + argc);

        for (size_t i = 0; i < arr.size(); ++i) {
            const std::string& flag = arr[i];

            if (flag == "-h" || flag == "--help") {
                args.help = true;
                return args;
            } else if (flag == "-w" || flag == "--width") {
                args.image_width = parse_uint(flag, next_value(arr, i));
            } else if (flag == "-s" || flag == "--samples-pp") {
                args.samples_per_pixel = parse_uint(flag, next_value(arr, i));
            } else if (flag == "--max-depth") {
                args.max_depth = parse_uint(flag, next_value(arr, i));
            } else if (flag == "--vfov") {
                args.vfov = parse_uint(flag, next_value(arr, i));
            } else if (flag == "--defocus-angle") {
                args.defocus_angle = std::stod(next_value(arr, i));
            } else if (flag == "--aspect-ratio") {
                args.aspect_ratio = std::stod(next_value(arr, i));
            } else if (flag == "--focus-dist") {
                args.focus_dist = std::stod(next_value(arr, i));
            } else if (flag == "--look-from") {
                args.lookfrom = Point3(next_value(arr, i));
            } else if (flag == "--look-at") {
                args.lookat = Point3(next_value(arr, i));
            } else if (flag == "--vup") {
                args.vup = Vector3(next_value(arr, i));
            }
        }

        return args;
    }

    static std::string usage(const std::string& prog) {
        const Args d;
        std::ostringstream out;
        out << "Usage: " << prog << " [options] > image.ppm\n"
            << "\n"
            << "Renders a scene and writes a PPM image to stdout.\n"
            << "\n"
            << "Options:\n"
            << "  -h, --help              Show this help and exit\n"
            << "  -w, --width N           Image width in pixels (default "
            << d.image_width << ")\n"
            << "  -s, --samples-pp N      Samples per pixel (default "
            << d.samples_per_pixel << ")\n"
            << "      --max-depth N       Max ray bounces (default "
            << d.max_depth << ")\n"
            << "      --aspect-ratio R    Width / height (default "
            << d.aspect_ratio << ")\n"
            << "      --vfov DEG          Vertical field of view (default "
            << d.vfov << ")\n"
            << "      --look-from X,Y,Z   Camera position (default "
            << format(d.lookfrom) << ")\n"
            << "      --look-at X,Y,Z     Point the camera looks at (default "
            << format(d.lookat) << ")\n"
            << "      --vup X,Y,Z         Camera up direction (default "
            << format(d.vup) << ")\n"
            << "      --defocus-angle DEG Aperture angle, 0 disables blur "
               "(default "
            << d.defocus_angle << ")\n"
            << "      --focus-dist D      Distance to the focus plane "
               "(default "
            << d.focus_dist << ")\n";
        return out.str();
    }

   private:
    static std::string format(const Vector3& v) {
        std::ostringstream out;
        out << v.X() << "," << v.Y() << "," << v.Z();
        return out.str();
    }

    static const std::string& next_value(const std::vector<std::string>& arr,
                                         size_t& i) {
        if (i + 1 >= arr.size()) {
            throw std::invalid_argument("Missing value for " + arr[i]);
        }
        return arr[++i];
    }

    static unsigned int parse_uint(const std::string& flag,
                                   const std::string& value) {
        long long n = std::stoll(value);
        if (n < 0) {
            throw std::invalid_argument(flag + " must be non-negative, got " +
                                        value);
        }
        if (n >
            static_cast<long long>(std::numeric_limits<unsigned int>::max())) {
            throw std::out_of_range(flag + " is too large: " + value);
        }
        return static_cast<unsigned int>(n);
    }
};

#endif