/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

// Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#include <iostream>
#include <string>
#include <fstream>

#include <glew/glew.h>
#include <GL/gl.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#include "logs.hpp"
#include "loadshader.hpp"
#include "glprogram.hpp"
#include "glinit.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool initGL();
void handleKeys(unsigned char key, int x, int y);
void update();
void render();
void close();

bool gRenderQuad = true;

SDL_Window *gWindow = NULL;
SDL_GLContext gContext;
SDL_GLContext *gPContext = new SDL_GLContext();

GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

/* bool initGL():
    create a gl program
        compilation and leaking of shaders, consequently assignment into the gl program
    create the data used by the program
        first the vertices raw
        then the indices of the vertices to form the tris (or other graphics primitives)
            (might seem weird but keeps you from having to store a lot of the vertices 3 or more times in your vertex buffer)
*/
bool initGL()
{
    gProgramID = CreateProgram();
    if (gProgramID == 1337)
    {
        std::cout << "Program failed to link" << std::endl;
    }

    // Get vertex attribute location
    gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
    if (gVertexPos2DLocation == -1)
    {
        printf("LVertexPos2D is not a valid glsl program variable!\n");
        return false;
    }

    glClearColor(0.f, 0.f, 0.f, 1.f);

    // VBO data
    GLfloat vertexData[] =
        {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f};

    // IBO data
    GLuint indexData[] = {0, 1, 2, 3};

    // Create VBO
    glGenBuffers(1, &gVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    // Create IBO
    glGenBuffers(1, &gIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

    return true;
}

void handleKeys(unsigned char key, int x, int y)
{
    // Toggle quad
    if (key == 'q')
    {
        gRenderQuad = !gRenderQuad;
    }
}

void update()
{
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (gRenderQuad)
    {
        // Bind program
        glUseProgram(gProgramID);

        // Enable vertex position
        glEnableVertexAttribArray(gVertexPos2DLocation);

        // Set vertex data
        glBindBuffer(GL_ARRAY_BUFFER, gVBO);

        // GLfloat g[] = {1.0f, 1.0f};
        glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

        // Set index data and render
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
        glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

        // Disable vertex position
        glDisableVertexAttribArray(gVertexPos2DLocation);

        // Unbind program
        glUseProgram(0);
    }
}

void cleanup()
{
    glDeleteProgram(gProgramID);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
    TTF_Quit();
}

int main(int argc, char *args[])
{
    if (!init(&gPContext, &gWindow, SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        printf("Failed to initialize!\n");
        cleanup();
        return false;
    }
    if (!initGL())
    {
        printf("Unable to initialize OpenGL!\n");
        return false;
    }

    bool quit = false;
    SDL_Event e;
    SDL_StartTextInput();
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_TEXTINPUT)
            {
                int x = 0, y = 0;
                SDL_GetMouseState(&x, &y);
                handleKeys(e.text.text[0], x, y);
            }
        }
        render();
        SDL_GL_SwapWindow(gWindow);
    }
    SDL_StopTextInput();
    cleanup();
    return 0;
}
