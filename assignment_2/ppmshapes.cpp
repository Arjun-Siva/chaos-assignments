#include <fstream>

int main() {
    const int width = 1920;
    const int height = 1080;

    std::ofstream out("shapes.ppm");
    out << "P3\n" << width << ' ' << height << "\n255\n";

    // white
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            out << 255<<" "<<255<<" "<<255<<"\n";
        }
    }



    out.close();
    return 0;
}
