# compile main project
# https://mesonbuild.com/Build-options.html#visual-studio
project('example', 'cpp')#, default_options: ['backend_startup_project=main'])

executable('main', 'main.cpp', 'project.cpp')


# meson configure -Doptimization=2 -Ddebug=false
# meson configure -Dbuildtype=release
# meson configure -Dbuildtype=debug