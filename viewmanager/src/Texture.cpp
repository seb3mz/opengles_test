#include "Texture.h"
#include <string.h>
#include "opencv2/imgproc/types_c.h"
#include <GLES3/gl32.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "imgtrans.h"

#define IMG_WIDTH 1152
#define IMG_HEIGHT 884

Texture::Texture(EglSurface *surface)
{
    xshader = new Shader();
    this->surface = surface;

    const char* vertexShaderSource = "/home/dve/work/code/pc/opengles_test/viewmanager/shader/vertex_wide.txt";
    const char* pszFragShader = "/home/dve/work/code/pc/opengles_test/viewmanager/shader/fragment_wide.txt";
    xshader->loadSource(vertexShaderSource, pszFragShader);
}

Texture::~Texture()
{
    if(!xshader)
    {
        delete xshader;
    }
}

void Texture::Draw()
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

    GLuint mTexID;
    glGenTextures(1, &mTexID);
    glBindTexture(GL_TEXTURE_2D, mTexID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, IMG_WIDTH, IMG_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pdwCamDataBGR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint  m_verticesVBO;
    glGenBuffers(1, &m_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    typedef struct  {
        float x;
        float y;
        float u;
        float v;
        float r;
        float g;
        float b;
    } PosUV;

    const PosUV datalist[] = {
        {-1.0, -1.0, 0, 1},
        {-1.0, 1.0, 0, 0},
        {1.0, -1.0, 1, 1},

        {1.0, -1.0, 1, 1},
        {-1.0, 1.0, 0, 0},
        {1.0, 1.0, 1, 0},
    };
    double PI = 3.1415926;
    double rotanglex = 0 * PI / 180.0f;
    double rotangley = 0 * PI / 180.0f;
    double rotanglez = 0 * PI / 180.0f;
    cv::Matx44f camRotMatX(1.0F, 0.0F, 0.0F, 0.0F,
			                0.0F, (float)cos(rotanglex), (float)(-sin(rotanglex)), 0.0F,
			                0.0F, (float)sin(rotanglex), (float)cos(rotanglex), 0.0F,
                            0.0F, 0.0F, 0.0F, 1.0F);
    cv::Matx44f camRotMatY((float)cos(rotangley), 0.0F, (float)sin(rotangley), 0.0F,
			                0.0F, 1.0F, 0.0F, 0.0F,
			                (float)(-sin(rotangley)), 0.0F, (float)cos(rotangley), 0.0F,
                            0.0F, 0.0F, 0.0F, 1.0F);
    cv::Matx44f camRotMatZ((float)cos(rotanglez), (float)(-sin(rotanglez)), 0.0F, 0.0F,
                           (float)sin(rotanglez), (float)cos(rotanglez), 0.0F, 0.0F,
                           0.0F, 0.0F, 1.0F, 0.0F,
                           0.0F, 0.0F, 0.0F, 1.0F);
    cv::Matx44f transMat(
                            1.0f, 0.0f, 0.0f, 0.5f,
                            0.0f, 1.0f, 0.0f, 0.5f,
                            0.0f, 0.0f, 1.0f, 0.5f,
                            0.0f, 0.0f, 0.0f, 1.0f
                        );

    //cv::Vec3f tran(0.0f, 0.0f, 0.0f);
    cv::Matx44f model = camRotMatZ * camRotMatY * camRotMatX * transMat;
    cv::Matx44f view = cv::Matx44f::eye();
    cv::Matx44f mvp = view * model;
    cv::Matx44f mt = mvp.t();

    glm::mat4 model_glm = glm::mat4(1.0f);
    model_glm = glm::translate(model_glm, glm::vec3(0.0f, 0.0f, 0.0f));
    //model_glm = glm::rotate(model_glm, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    model_glm = glm::scale(model_glm, glm::vec3(2.0f, 2.0f, 2.0f));

    //cv::Affine3f VCamAff = cv::Affine3f(camRotMatZ * camRotMatY * camRotMatX, tran);
    //cv::Affine3f VCamAff = cv::Affine3f::Identity().rotate(cv::Vec3f(0.0f, 0.0f, 0.0f)).translate(cv::Vec3f(0.3f, 0.0f, 0.0f));
    //cv::Affine3f VCamAff = cv::Affine3f::Identity().scale(cv::Vec3f(2.0f, 1.0f, 1.0f));
    glUseProgram(xshader->shaderProgram);
    //如果不加glUseProgram这句，渲染出来的图像是黑色的，错误码是1282
    GLint mvp_v = glGetUniformLocation(xshader->shaderProgram, "uMVPMat");
    cout << glGetError() << endl;
    //glUniformMatrix4fv(mvp_v, 1, GL_FALSE, mt.val);
    glUniformMatrix4fv(mvp_v, 1, GL_FALSE, glm::value_ptr(model_glm));
    cout << glGetError() << endl;
    GLuint locAttrPos = glGetAttribLocation(xshader->shaderProgram, "inPosition");
    GLuint locAttrUV = glGetAttribLocation(xshader->shaderProgram, "inTexCoord");
    GLuint locUniTxt = glGetUniformLocation(xshader->shaderProgram, "texture0");
    glUniform1i(locUniTxt, 0);
    #define BUFFER_OFFSET(i) ((char *)NULL + (i))

    glBindBuffer(GL_ARRAY_BUFFER, m_verticesVBO);
    GLsizei stride = sizeof(PosUV);
    glVertexAttribPointer(locAttrPos, 2, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(locAttrPos);

    glVertexAttribPointer(locAttrUV, 2, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(locAttrUV);

    glBufferData(GL_ARRAY_BUFFER, sizeof(datalist), datalist, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //fix bug: Forces the default active texture to be activated on Qnx
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexID);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    float borderColor[] = {0.0f,0.0f,0.0f,1.0f};
    glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

    xshader->use();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    surface->SwapBuffers();
    getchar();

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(0);
}