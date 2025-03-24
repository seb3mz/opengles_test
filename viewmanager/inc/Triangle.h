#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <iostream>
#include "Shader.h"
#include "EglSurface.h"
using namespace std;

class Triangle
{
public:
    Triangle(EglSurface *surface);
    ~Triangle();
    void Draw();

private:
    unsigned int VBO, VAO;
    Shader *xshader;
    EglSurface *surface;
};

#endif //TRIANGLE_H