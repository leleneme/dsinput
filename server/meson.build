project('dsinput', 'cpp',
    version : '0.1',
    default_options : ['warning_level=everything', 'cpp_std=c++23'])

if not (build_machine.system() == 'linux')
    error('This program currently only supports Linux.')
endif

sources = ['src/main.cpp', 'src/vcontroller_linux.cpp', 'src/udp_server_linux.cpp']

executable('server',
    sources: sources,
    include_directories: 'inc/',
    native : true)