#ifndef RENDERPPM_H
#define RENDERPPM_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include "camera.h"
#include "ray.h"
#include "color.h"
#include "triangle.h"

double EPSILON = 1e-6;

void renderScene(const std::string &filename, int width, int height, Camera& camera, const std::vector<Triangle>& triangles, Color bg)
{
    std::ofstream out(filename);
    out << "P3\n" << width << ' ' << height << "\n255\n";

    // for each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            float u = (x + 0.5f) / width;
            float v = (y + 0.5f) / height;

            Ray ray = camera.generateRay(u, v);
            Color pixelColor = bg;

            double shortest_intersection = 1/EPSILON;

            for(const Triangle& t: triangles) {
                double p = t.intersect(ray);
                if (p < shortest_intersection && p > EPSILON) {
                    shortest_intersection = p;
                    pixelColor = t.color;
                }
            }

            out<<static_cast<int>(pixelColor.r * 255)<<" "<<static_cast<int>(pixelColor.g * 255)<<" "<<static_cast<int>(pixelColor.b * 255)<<"\n";
        }
    }

    out.close();
}

#endif // RENDERPPM_H
