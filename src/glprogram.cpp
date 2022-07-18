#include <iostream>

#include <glew/glew.h>
#include <GL/gl.h>

#include "logs.hpp"
#include "loadshader.hpp"

GLuint CreateProgram()
{
    /*
        create a gl program
            define compile and set vertex shader
            define compile and set fragment shader
            link them to make the program
    */

    GLuint gProgramID = glCreateProgram();

    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertex_shader_str = ReadFile("./src/vertex.glsl");
    const char *vertex_shader_source = vertex_shader_str.c_str();
    glShaderSource(vertexShader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertexShader);
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE)
    {
        printf("Unable to compile vertex shader %d!\n", vertexShader);
        printShaderLog(vertexShader);
        return 1337;
    }
    glAttachShader(gProgramID, vertexShader);

    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragment_shader_str = ReadFile("./src/fragment.glsl");
    const char *fragment_shader_source = fragment_shader_str.c_str();
    glShaderSource(fragmentShader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragmentShader);
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
    if (fShaderCompiled != GL_TRUE)
    {
        printf("Unable to compile fragment shader %d!\n", fragmentShader);
        printShaderLog(fragmentShader);
        return 1337;
    }
    glAttachShader(gProgramID, fragmentShader);

    // link
    glLinkProgram(gProgramID);
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE)
    {
        printf("Error linking program %d!\n", gProgramID);
        printProgramLog(gProgramID);
        return 1337;
    }

    return gProgramID;
}