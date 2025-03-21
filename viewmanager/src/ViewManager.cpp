#include <iostream>
#include "ViewManager.h"
#include <EGL/egl.h>
//#include <GLES2/gl2.h>
#include <GLES3/gl32.h>
#include <string.h>
#include "opencv2/imgproc/types_c.h"

using namespace std;

const char *vertexShaderSource = "\
                    attribute highp vec3 inVertex; \
                    void main(void) \
                    { \
                        gl_Position = vec4(inVertex, 1.0); \
                    }";
const char* pszFragShader = "\
                    void main(void)\
                    {\
                        gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \
                    }";
#define IMG_WIDTH 1152
#define IMG_HEIGHT 884
    

ViewManager::ViewManager(XWindow *win, EglSurface *sur)
{
    window = win;
    surface = sur;
    xshader = new Shader();
}

ViewManager::~ViewManager()
{
}

void ViewManager::PreRender()
{
    //创建窗口
    window->CreateWindow("opengl test", IMG_WIDTH, IMG_HEIGHT);
    //创建 surface
    surface->CreateSurface(window->GetEGLNativeWindow());
    //创建 context
    surface->CreateEglContext();
    surface->StartMakeCurrent();
}

void ViewManager::Render()
{
    cout << "Begin Render" << endl;
    glViewport(0, 0, IMG_WIDTH, IMG_HEIGHT);

    //DrawTriangle();
    DrawTexture();
}

void ViewManager::DrawTriangle()
{
    //加载 shader 
    //xshader->loadSource(vertexShaderSource, pszFragShader);
    const char* vertexShaderSource = "/home/dve/work/code/pc/opengl/viewmanager/shader/vertex.txt";
    const char* pszFragShader = "/home/dve/work/code/pc/opengl/viewmanager/shader/fragment.txt";
    xshader->loadSource(vertexShaderSource, pszFragShader);

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

void ViewManager::DrawTexture()
{
    unsigned char* m_pbyData = new unsigned char[IMG_WIDTH*IMG_HEIGHT*2];
    (void)memset(m_pbyData, 0, IMG_WIDTH * IMG_HEIGHT * 2);

    const char* img_path = "/home/dve/work/code/360/isvs/resource/B233_MY25/DebugRes/Patac_Board/back.dat";
    int m_dwFileSize = 0;
    FILE *m_pFile = fopen(img_path,"rb");
    if(m_pFile)
    {
        (void)fseek(m_pFile, 0, SEEK_END);
		m_dwFileSize = ftell(m_pFile);
		(void)fseek(m_pFile, 0, SEEK_SET);
        cout << "文件大小:" << m_dwFileSize << endl;
        (void)fread(m_pbyData, 1, IMG_WIDTH*IMG_HEIGHT*2, m_pFile);
        fclose(m_pFile);
    }
    else
    {
        cout << "读取文件:" << img_path << "失败!!!" << endl;
        return;
    }

    unsigned char *m_pdwCamDataBGR = new AVM_Byte[IMG_WIDTH * IMG_HEIGHT * 3];
    cv::Mat imageUYVY(IMG_HEIGHT,IMG_WIDTH, CV_8UC2, m_pbyData);
    cv::Mat image(IMG_HEIGHT,IMG_WIDTH, CV_8UC3, m_pdwCamDataBGR);
    cvtColor(imageUYVY, image, cv::COLOR_YUV2RGB_UYVY);
    // unsigned char* pRGBData  = new unsigned char[IMG_WIDTH * IMG_HEIGHT * 2];
    // ConvertUYVYToRGB565(m_pbyData, IMG_WIDTH, IMG_HEIGHT, pRGBData);
    // unsigned char* m_pdwCamData  = new unsigned char[IMG_WIDTH * IMG_HEIGHT * 3];
    // ConvertUYVYToRGB888(m_pbyData, IMG_WIDTH, IMG_HEIGHT, m_pdwCamData);

    // cv::Mat tmpMat;
    // cv::Mat tmpBlendMat;
    // tmpMat = cv::Mat(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, m_pdwCamData);
    // tmpBlendMat = tmpMat(cv::Range(tmpMat.rows / 2, tmpMat.rows), cv::Range(0, tmpMat.cols));

    // std::vector<cv::Mat> Channels0;
    // cv::cvtColor(tmpMat, tmpMat, CV_RGB2YUV);
    // cv::split(tmpBlendMat, Channels0);
    // const AVM_Double r0 = cv::mean(Channels0[0])[0];
    // cv::cvtColor(tmpMat, tmpMat, CV_YUV2RGB);
    // ConvertRGB888ToRGB565(m_pdwCamData, IMG_WIDTH, IMG_HEIGHT, pRGBData);

    // GLuint texID;
    // glGenTextures(1, &texID);
    // glBindTexture(GL_TEXTURE_2D, texID);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMG_WIDTH, IMG_HEIGHT, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, pRGBData);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}