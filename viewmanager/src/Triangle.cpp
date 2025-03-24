#include "Triangle.h"
#include <EGL/egl.h>
//#include <GLES2/gl2.h>
#include <GLES3/gl32.h>
#include <iostream>
using namespace std;

Triangle::Triangle(EglSurface *sur)
{
    xshader = new Shader();
    surface = sur;

    const char* vertexShaderSource = "/home/dve/work/code/pc/opengles_test/viewmanager/shader/vertex.txt";
    const char* pszFragShader = "/home/dve/work/code/pc/opengles_test/viewmanager/shader/fragment.txt";
    xshader->loadSource(vertexShaderSource, pszFragShader);
}
Triangle::~Triangle()
{
    if(!xshader)
    {
        delete xshader;
    }
}

void Triangle::Draw()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 



    while(1)
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        xshader->use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        surface->SwapBuffers();
        getchar();
    }

    glDisableVertexAttribArray(0);
}