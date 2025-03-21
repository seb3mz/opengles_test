#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
    Shader();
    ~Shader();

    bool loadSource(const char *vertexShaderSource, const char* pszFragShader);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    unsigned int shaderProgram;

private:
    void checkCompileErrors(unsigned int shader, std::string type); 
};

#endif  //SHADER_H