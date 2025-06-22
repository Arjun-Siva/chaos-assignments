#include <fstream>

int main() {
    const int width = 1920;
    const int height = 1080;
    const int ncols = 5;
    const int nrows = 4;

    std::ofstream out("rectangles.ppm");
    out << "P3\n" << width << ' ' << height << "\n255\n";

    int row = 0;
    int col = 0;

    for (int y = 0; y < height; ++y) {
        row = y/(height/nrows);
        for (int x = 0; x < width; ++x) {
            col = x / (width/ncols);
            int r = (31 * row + 41 * col + (row*col*col)) % 256;
            int g = (7 * row + 19 * col + (row*col*col)) % 256;
            int b = (73 * row + 77 * col + (row*col*col)) % 256;
            out << r<<" "<<g<<" "<<b<<"\n";
        }
    }

    out.close();
    return 0;
}
