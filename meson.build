project(
    'wayland-logout',
    'c',
    version: '1.0',
    license: 'MIT',
    meson_version: '>=0.55.0',
)

executable('wayland-logout', 'wayland-logout.c', install : true)

install_data('wayland-logout.1', install_dir :
    join_paths(get_option('prefix'), get_option('mandir'), 'man1'))
