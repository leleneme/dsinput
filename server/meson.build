project('dsinput', 'cpp',
    version : '0.1',
    default_options : ['warning_level=everything', 'cpp_std=c++23'])

sources = ['src/server.cpp']

if build_machine.system() == 'linux'
    sources += 'src/vcontroller_linux.cpp'
    sources += 'src/udp_server_linux.cpp'
else
    error('Virtual controller and UDP server implementations are only avaliable on Linux.')
endif

executable('server',
    sources: sources,
    include_directories: 'inc/',
    native : true)