#include <stdlib.h>
#include <stdio.h>
#include "vec.h"
#include "ray.h"
#include "common.h"
#include "hittable.h"
#include "hittable_arr.h"
#include "color.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <chrono>
#include "flops.h"

float hit_sphere(const Ray& r, const Point3& center, float radius) {

    // Vector3 oc = center - r.origin();
    // auto a = r.direction().dot(r.direction());
    // auto b = -2.0 * (r.direction().dot(oc));
    // auto c = (oc.dot(oc)) - radius*radius;
    // auto discriminant = b*b - 4*a*c;

    Vector3 oc = center - r.origin();
    auto a = r.direction().len(); a *= a;
    auto h = r.direction().dot(oc);
    auto c = oc.len(); c *= c;
    c -= radius*radius;
    auto discriminant = (h*h) - (a*c);

    if ( discriminant < 0 ) {
        return -1.0;
    } else {
        return (h - std::sqrt(discriminant)) / a;
    }


}

color ray_color(const Ray& r, const HittableList& world) {

    HitRecord rec;
    if (world.hit(r, 0, inf, rec)) {
        return (rec.normal + color(1,1,1)) * 0.5;
    }

    Vector3 unit_dir = r.direction().norm();

    auto a = (unit_dir.y + 1.0) * 0.5;
    return color(1.0, 1.0, 1.0)*(1.0 - a) + color(0.5, 0.7, 1.0)*a;

}

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <output_file>" << std::endl;
        return 1;
    }

    std::ofstream output_file(argv[1]);
    if (!output_file) {
        std::cerr << "Error: Could not open file " << argv[1] << " for writing." << std::endl;
        return 1;
    }

    std::clog << "Hello, World!" << std::endl;

    // Initialize world

    HittableList world;

    world.add(std::make_shared<Sphere>(Point3(0,-100.5,-1), 100));
    world.add(std::make_shared<Sphere>(Point3(0,0,-1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(-0.7, 0.2, -1.5), 0.3));
    world.add(std::make_shared<Sphere>(Point3(0.5, -0.3, -0.8), 0.2));
    world.add(std::make_shared<Sphere>(Point3(0.3, 0.4, -1.2), 0.25));

    // Initialize screen

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 640;
    int image_height = int(image_width / aspect_ratio);
    image_height = image_height == 0 ? 1 : image_height;

    // Camera properties

    auto focal_length = 1.0;
    auto camera_center = Point3(0, 0, 0);

    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);
    
    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    Vector3 viewport_u = Vector3(viewport_width, 0, 0);
    Vector3 viewport_v = Vector3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    Vector3 pixel_delta_u = viewport_u / image_width;
    Vector3 pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    Vector3 viewport_upper_left = camera_center - Vector3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    Vector3 pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;

    std::clog << "Generating image..." << std::endl;

    unsigned long long render_time_total = 0;
    unsigned long long write_time_total = 0;
    int pixel_cnt = 0;
    unsigned long bytes_tx = 0;

    std::string image_str;
    image_str.reserve(12 * image_height * image_width + 64);
    std::stringstream image_stream(image_str);
    image_stream << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        if ( j % 10 == 0 ) {

            // clear screen
            for ( int i = 0; i < 5; i++ ) { std::clog << "\033[F\033[J"; }
            std::clog << "Image generation: \n\t" << 100.f * float(float(j)/image_height) << "% complete\n";
            std::clog << "\t" << (float(render_time_total - write_time_total)/float(pixel_cnt)) << " Pixels rendered /s\n";
            std::clog << "\t" << (1e-6 * float(bytes_tx)/(float(write_time_total * 1e-9))) << " MB/s (" << float(bytes_tx)/1e6 << " MB saved)" << "\n";
            std::clog << "\t" << float(flops_total)/float(1e6) << " Mflops Performed" << std::endl;

        }
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < image_width; i++) {

            auto pixel_center = pixel00_loc + (pixel_delta_u * i) + (pixel_delta_v * j);
            auto ray_direction = pixel_center - camera_center;
            Ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, world);

            int bytes_tx_this_write;
            write_time_total += writeColor(image_stream, pixel_color, bytes_tx_this_write);
            bytes_tx += bytes_tx_this_write;
            
        }
        auto stop = std::chrono::high_resolution_clock::now();
        render_time_total += std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
        pixel_cnt ++;

    }

    image_stream.flush();

    output_file.write(image_stream.str().c_str(), image_stream.str().length());
    output_file.close();

}