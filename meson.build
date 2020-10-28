project(
    'wayland-logout',
    'c',
    version: '1.0',
    license: 'MIT',
    meson_version: '>=0.55.0',
)

impl = get_option('implementation')

if impl == 'c'
    executable('wayland-logout', 'wayland-logout.c', install : true)
elif impl == 'python'
    custom_target('python',
        input : ['wayland-logout.py'],
        output : ['wayland-logout'],
        command : ['cp', '@INPUT@', '@OUTPUT@'],
        install : true,
        install_dir : join_paths(get_option('prefix'), get_option('bindir'))
    )
elif impl == 'shell'
    custom_target('shell',
        input : ['wayland-logout.sh'],
        output : ['wayland-logout'],
        command : ['cp', '@INPUT@', '@OUTPUT@'],
        install : true,
        install_dir : join_paths(get_option('prefix'), get_option('bindir'))
    )
endif
