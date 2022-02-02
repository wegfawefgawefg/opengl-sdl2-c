#include <iostream>
#include <string>

#include <glew/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

static const int SCALE = 100;
static const int WINDOW_WIDTH = 16 * SCALE;
static const int WINDOW_HEIGHT = 9 * SCALE;

void draw_text(
        SDL_Renderer *renderer, 
        TTF_Font *font, 
        char *text,
        SDL_Color color, 
        int x, int y) {
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect text_rect = {x, y, text_surface->w, text_surface->h};
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

struct dimensions {
    int width, height;
};

glm::mat4 transform(glm::vec2 const& Orientation, glm::vec3 const& Translate, glm::vec3 const& Up)
{
    glm::mat4 Proj = glm::perspective(glm::radians<float>(45.f), 1.33f, 0.1f, 10.f);
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.f), Translate);
    glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, Orientation.y, Up);
    glm::mat4 View = glm::rotate(ViewRotateX, Orientation.x, Up);
    glm::mat4 Model = glm::mat4(1.0f);
    return Proj * View * Model;
}

void printProgramLog( GLuint program );
void printShaderLog( GLuint shader );

GLuint gProgramID = 0;
GLint gVertexPos2DLocation = -1;
GLuint gVBO = 0;
GLuint gIBO = 0;

void printProgramLog( GLuint program )
{
    //Make sure name is shader
    if( glIsProgram( program ) )
    {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        //Get info string length
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
        
        //Allocate string
        char* infoLog = new char[ maxLength ];
        
        //Get info log
        glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            //Print Log
            printf( "%s\n", infoLog );
        }
        
        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a program\n", program );
    }
}

void printShaderLog( GLuint shader )
{
    //Make sure name is shader
    if( glIsShader( shader ) )
    {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;
        
        //Get info string length
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
        
        //Allocate string
        char* infoLog = new char[ maxLength ];
        
        //Get info log
        glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
        if( infoLogLength > 0 )
        {
            //Print Log
            printf( "%s\n", infoLog );
        }

        //Deallocate string
        delete[] infoLog;
    }
    else
    {
        printf( "Name %d is not a shader\n", shader );
    }
}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    
    //Render quad
    if( gRenderQuad )
    {
        //Bind program
        glUseProgram( gProgramID );

        //Enable vertex position
        glEnableVertexAttribArray( gVertexPos2DLocation );

        //Set vertex data
        glBindBuffer( GL_ARRAY_BUFFER, gVBO );
        glVertexAttribPointer( gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL );

        //Set index data and render
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
        glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );

        //Disable vertex position
        glDisableVertexAttribArray( gVertexPos2DLocation );

        //Unbind program
        glUseProgram( NULL );
    }
}

int main() {
    // glm::vec3 a = glm::vec3(1, 1, 1);
    // glm::vec3 b = glm::vec3(2, 2, 2);
    // auto c = a + b;
    // std::cout << glm::to_string(c) << std::endl;

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    window = SDL_CreateWindow("opengl-sdl-cpp", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 16);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_ShowCursor(SDL_DISABLE);

    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    if(glcontext == NULL){
        printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
    } 
    else{
        glewExperimental = GL_TRUE; 
        GLenum glewError = glewInit();
        if(glewError != GLEW_OK){
            printf( "Error initializing GLEW! %s\n", glewGetErrorString( glewError ) );
        }
        if(SDL_GL_SetSwapInterval( 1 ) < 0){  //vsync
            printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
        }
        // if(!initGL()){
        //     printf( "Unable to initialize OpenGL!\n" );
        // }
    }

    printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), 
    glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Load font
    TTF_Font *font = TTF_OpenFont("./assets/FreeSans.ttf", 24);
    if (font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    // setup for delta time
    Uint64 frame_time_now = SDL_GetPerformanceCounter();
    Uint64 frame_time_last = 0;
    double dt = 0;

    SDL_Event event;
    while (1) {
        // update delta time
        frame_time_last = frame_time_now;
        frame_time_now = SDL_GetPerformanceCounter();
        dt = (double)((frame_time_now - frame_time_last) / (double)SDL_GetPerformanceFrequency() );
        const double fps = 1.0 / dt;   

        // events
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }

        // keys
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_Q]) {
            break;
        }

        // clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // draw mouse location
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        int x, y;
        SDL_GetMouseState(&x, &y);
        SDL_Rect mouse_rect = {x, y, 8, 8};
        SDL_RenderFillRect(renderer, &mouse_rect);

        // draw fps
        const SDL_Color color = {255, 255, 255, 255};
             
        char fps_str[32];
        sprintf(fps_str, "%d", (int)fps);
        draw_text(renderer, font, fps_str, color, 0, 0);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;

}
