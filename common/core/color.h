#ifndef COLOR_H
#define COLOR_H


class Color
{
public:
    float r;
    float g;
    float b;

    Color();
    Color(float r, float g, float b);

    Color operator*(float scalar) const;
    friend Color operator*(float scalar, const Color& c);
    Color operator+(const Color& other);
};

#endif // COLOR_H
