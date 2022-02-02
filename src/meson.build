project('main', 'cpp',
    default_options: 'default_library=static'
)

sdl2_dep = dependency('sdl2')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_image_dep = dependency('SDL2_image')
glew_dep = dependency('glew')
gl_dep = dependency('gl')

glm_dep = include_directories('glm')

executable('main', 
    sources: [
        'main.cpp',
        'logs.cpp', 'logs.hpp',
        'loadshader.cpp', 'loadshader.hpp',
        'glprogram.cpp', 'glprogram.hpp',
        'glinit.cpp', 'glinit.hpp',
    ],
    include_directories: [
        glm_dep,
    ],
    dependencies: [
        sdl2_dep,
        sdl2_image_dep,
        sdl2_ttf_dep,
        glew_dep,
        gl_dep,
    ],
)