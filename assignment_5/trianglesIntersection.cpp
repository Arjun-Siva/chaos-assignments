#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "triangle.h"
#include <fstream>
#include <vector>
#include <iostream>


int main() {
    const int width = 1920;
    const int height = 1080;

    std::vector<Ray*> rays;
    std::vector<Triangle*> triangles;

    Triangle t1(vec3(-2,1,-5), vec3(2, 1, -5), vec3(0, 3, -5));
    t1.color = Color(255, 0, 0);
    triangles.push_back(&t1);

    Triangle t2(vec3(0,2,-7), vec3(4, 2, -7), vec3(2, 5, -7));
    t2.color = Color(0, 255, 0);
    triangles.push_back(&t2);

    Triangle t3(vec3(-2,-1,-4), vec3(0, 2, -7), vec3(-1, 5, -7));
    t3.color = Color(0, 0, 255);
    triangles.push_back(&t3);


    std::ofstream out("manyTriangles.ppm");
    out << "P3\n" << width << ' ' << height << "\n255\n";

    // for each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float sx = x;
            float sy = y;

            sx += 0.5; sy += 0.5;

            // raster to NDC [0.0, 1.0]
            sx /= width;
            sy /= height;

            // NDC to screen [-1.0, 1.0]
            sx = (2.0 * sx) - 1.0;
            sy = 1.0 - (2.0 * sy);

            // scale for aspect ratio
            sx *= (width / height);

            Ray* ray = new Ray{vec3(0, 0, 0), vec3(sx, sy, -1).normalized()};
            double shortest_intersection = 100000;
            Color *pixelColor = new Color(0, 0, 0);

            for(Triangle* t: triangles) {
                double p = t->intersect(*ray);
                if (p < shortest_intersection && p >= 0) {
                    shortest_intersection = p;
                    *pixelColor = t->color;
                }
            }

            out<<pixelColor->r<<" "<<pixelColor->g<<" "<<pixelColor->b<<"\n";
        }
    }

    out.close();
    return 0;
}
