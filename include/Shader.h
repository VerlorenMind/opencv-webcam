#ifndef OPENCV_LAB_SHADER_H
#define OPENCV_LAB_SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

class Shader {
private:
    GLenum type;
    std::string name;
public:
    GLuint id;

    enum SourceType {
        fromString, fromFile
    };

    Shader (const std::string name, GLenum type, Shader::SourceType sourceType, const GLchar *source);
    ~Shader ();
};


typedef std::vector<Shader *> ShaderList;



class ShaderProgram {
private:
    std::string name;

public:
    GLuint id;

    void use ();

    ShaderProgram (const std::string name, const ShaderList &shaderList);
    ~ShaderProgram ();
};

#endif //OPENCV_LAB_SHADER_H