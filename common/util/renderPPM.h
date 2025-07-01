#ifndef RENDERPPM_H
#define RENDERPPM_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "camera.h"
#include "ray.h"
#include "color.h"
#include "triangle.h"

double EPSILON = 1e-6;

void renderScene(std::string &filename, int width, int height, Camera& camera, const std::vector<Triangle*>& triangles)
{
    std::ofstream out(filename);
    out << "P3\n" << width << ' ' << height << "\n255\n";

    // for each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            float u = (x + 0.5f) / width;
            float v = (y + 0.5f) / height;

            Ray ray = camera.generateRay(u, v);

            double shortest_intersection = 1/EPSILON;
            Color *pixelColor = new Color(0, 0, 0);

            for(Triangle* t: triangles) {
                double p = t->intersect(ray);
                if (p < shortest_intersection && p > EPSILON) {
                    shortest_intersection = p;
                    *pixelColor = t->color;
                }
            }

            out<<pixelColor->r<<" "<<pixelColor->g<<" "<<pixelColor->b<<"\n";
        }
    }

    out.close();
}

#endif // RENDERPPM_H
