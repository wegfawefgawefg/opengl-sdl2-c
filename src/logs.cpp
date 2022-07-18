#include <glew/glew.h>
#include <GL/gl.h>
#include <stdio.h>

#include "logs.hpp"

void printProgramLog(GLuint program)
{
    // Make sure name is shader
    if (glIsProgram(program))
    {
        // Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        // Get info string length
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // Allocate string
        char *infoLog = new char[maxLength];

        // Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            // Print Log
            printf("%s\n", infoLog);
        }

        // Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf("Name %d is not a program\n", program);
    }
}

void printShaderLog(GLuint shader)
{
    // Make sure name is shader
    if (glIsShader(shader))
    {
        // Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        // Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // Allocate string
        char *infoLog = new char[maxLength];

        // Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0)
        {
            // Print Log
            printf("%s\n", infoLog);
        }

        // Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf("Name %d is not a shader\n", shader);
    }
}