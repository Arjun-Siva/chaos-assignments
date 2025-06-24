#include "../common/2dShapes/shapes.h"
#include <fstream>
#include <vector>


int main() {
    const int width = 1920;
    const int height = 1080;

    std::vector<Shape*> shapes;

    shapes.push_back(new Circle(500, 500, 100, 255, 0, 0));
    shapes.push_back(new Heart(800, 300, 50.0f, 0, 255, 0));
    shapes.push_back(new Heart(100, 900, 70.0f, 0, 0, 255));
    shapes.push_back(new Heart(1000, 900, 200.0f, 100, 25, 10));

    std::ofstream out("shapes.ppm");
    out << "P3\n" << width << ' ' << height << "\n255\n";

    // for each pixel
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool hit = false;
            for (const auto *shape : shapes) {
                if (shape->pixelInside(x, y)) {
                    out << shape->r<<" "<<shape->g<<" "<<shape->b<<"\n";
                    hit = true;
                    break;
                }
            }
            if (!hit) out << 255<<" "<<255<<" "<<255<<"\n"; // background
        }
    }

    out.close();
    return 0;
}
