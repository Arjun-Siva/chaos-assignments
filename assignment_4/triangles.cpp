#include <iostream>
#include "triangle.h"
#include "vec3.h"

int main () {
    Triangle t1(vec3(-1.75, -1.75, -3), vec3(1.75, -1.75, -3), vec3(0, 1.75, -3));
    Triangle t2(vec3(0, 0, -1), vec3(1, 0, 1), vec3(-1, 0, 1));
    Triangle t3(vec3(0.56, 1.11, 1.23), vec3(0.44, -2.368, -0.54), vec3(-1.56, 0.15, -1.92));

    std::cout<<"t1:"<<t1.normal<<std::endl;
    std::cout<<"t2:"<<t2.normal<<std::endl;
    std::cout<<"t3:"<<t3.normal<<std::endl;
    return 0;
}

