project('main', 'cpp',
    default_options: [
        'default_library=static',
        'optimization=3',
        'cpp_std=c++17',
        'warning_level=3'
    ],
)

message('optimization level is ' + get_option('optimization'))
message('standard: ' + get_option('cpp_std'))

sdl2_dep = dependency('sdl2')
sdl2_ttf_dep = dependency('SDL2_ttf')
sdl2_image_dep = dependency('SDL2_image')
glew_dep = dependency('glew')
gl_dep = dependency('gl')

executable('main', 
    sources: [
        'src/main.cpp',
        'src/logs.cpp', 
        'src/loadshader.cpp',
        'src/glprogram.cpp',
        'src/glinit.cpp',
    ],
    include_directories: [
        'src/headers',
        'src/libs',
    ],
    dependencies: [
        sdl2_dep,
        sdl2_image_dep,
        sdl2_ttf_dep,
        gl_dep,
        glew_dep,
    ],
)