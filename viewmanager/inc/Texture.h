#ifndef TEXTURE_H
#define TEXTURE_H
#include <iostream>
#include "Shader.h"
#include "EglSurface.h"
using namespace std;

class Texture
{
public:
    Texture(EglSurface *surface);
    ~Texture();
    void Draw();

private:
    unsigned int VBO, VAO;
    Shader *xshader;
    EglSurface *surface;
};

#endif //TEXTURE_H