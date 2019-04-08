#include "Logger.h"
#include "Shader.h"
#include "utility.h"

void ShaderProgram::use () {
    glUseProgram (id);
}

Shader::Shader(const std::string name, GLenum type, Shader::SourceType sourceType, const GLchar *source) {
    this->name = name;
    this->type = type;

    this->id = glCreateShader(type);
    switch(sourceType) {
        case fromString: {
            glShaderSource (id, 1, &source, NULL);
            glCompileShader (id);
        }
        case fromFile: {
            bool loadSuccess = false;
            std::string loaded = loadFileToString(source, loadSuccess);
            if (!loadSuccess) {
                logger.write("Error loading shader\n");
            }
            const GLchar *loadedStr = loaded.c_str ();
            glShaderSource (id, 1, &loadedStr, NULL);
            glCompileShader (id);
        }
    }

    GLint success = 0;
    glGetShaderiv (id, GL_COMPILE_STATUS, &success);
    if (!success) {
        logger.write("Shader failed to compile");
    }

}

ShaderProgram::ShaderProgram (const std::string name, const ShaderList &shaderList) {
    GLint success = 0;
    std::string log;

    this->name = name;

    // Link program
    id = glCreateProgram ();
    for (auto shader : shaderList)
        glAttachShader (id, shader->id);
    glLinkProgram (id);

    // Check link status
    glGetProgramiv (id, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logSize = 0;
        glGetProgramiv (id, GL_INFO_LOG_LENGTH, &logSize);
        log.resize (logSize);
        glGetProgramInfoLog (id, logSize, NULL, &log[0]);
        logger.write("Shader program \"%s\" error: %s", name.c_str (), log.c_str ());
    } else
        logger.write("Shader program \"%s\" linked", name.c_str ());
}

Shader::~Shader() {
    glDeleteShader (id);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram (id);
}