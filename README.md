# raytracer

A C++ ray tracer written for learning purposes, following [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## Usage

Requires a C++17 compiler and `make`.

```sh
make raytracer                       # build to build/raytracer
./build/raytracer > img.ppm          # render the scene to a PPM image
make run                             # clean, build and render in one step
make test                            # build and run the tests
```

Open `img.ppm` with any PPM-capable viewer (e.g. Preview on macOS, GIMP).

Scene and camera settings (resolution, samples per pixel, field of view, depth of field) are set in `src/main.cpp`.

## Known limitations

- Single-threaded, so high-quality renders are slow.
- Only spheres are supported.
- Output is plain PPM written to stdout; no other image formats.
- Scene and camera are hardcoded in `main.cpp`; there is no scene file or command-line configuration.
- Covers only the first book of the series (no BVH, textures, lights, or quads).
