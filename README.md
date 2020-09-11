# wayland-logout

wayland-logout is a simple program that sends SIGINT signal to a wayland compositor by looking up the pid for the wayland socket file. The path to the socket file is derived from WAYLAND_DISPLAY and XDG_RUNTIME_DIR environment variables. There are C and python and sh implementations in this repo. Use meson to build an executable from the source. The python and sh scripts are standalone. These can be used to kill a specific compositor instance and can be used in programs such as [wlogout](https://github.com/ArtsyMacaw/wlogout). For FreeBSD, only wayland-logout.sh will work since the others are Linux-specific implementations.

![logout](/wayland-logout.png)
