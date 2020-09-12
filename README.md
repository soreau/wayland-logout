# wayland-logout

wayland-logout is a simple program that sends SIGINT to a wayland compositor by looking up the pid for the wayland socket file. The path to the socket file is derived from WAYLAND_DISPLAY and XDG_RUNTIME_DIR environment variables. There are C and python and shell implementations in this repo. Use `-Dimplementation=$impl` to choose which implementation to use. The default is `shell` since it works on FreeBSD and Linux (the others only work on Linux). This can be used to kill a specific compositor instance and can be used in programs such as [wlogout](https://github.com/ArtsyMacaw/wlogout).

![logout](/wayland-logout.png)
