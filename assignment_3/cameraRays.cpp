#include "vec3.h"
#include <fstream>
#include <vector>

struct Ray {
    vec3 origin;
    vec3 direction;
};


int main() {
    const int width = 1920;
    const int height = 1080;

    std::vector<Ray*> rays;

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
            rays.push_back(ray);
        }
    }

    std::ofstream out("cameraRays.ppm");
    out << "P3\n" << width << ' ' << height << "\n255\n";

    // coloring
    for(auto ray:rays) {
        double dx = ray->direction.x;
        double dy = ray->direction.y;
        double dz = ray->direction.z;

        out << abs(static_cast<int>(255*dx)) <<" "<< abs(static_cast<int>(255*dy)) <<" "<< abs(static_cast<int>(255*dz))<<"\n";
    }

    out.close();
    return 0;
}
