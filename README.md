# wayland-logout

wayland-logout is a simple utility that sends SIGINT to a wayland compositor by looking up the pid for the wayland socket file. The path to the socket file is derived from WAYLAND_DISPLAY and XDG_RUNTIME_DIR environment variables. There are `c` and `python` and `shell` implementations in this repository. Use `-Dimplementation=$impl` to choose which implementation to use. The default is `shell` since it works on FreeBSD and Linux (the others only work on Linux). This can be used to kill a specific compositor instance and can be used in programs such as [wlogout](https://github.com/ArtsyMacaw/wlogout) or to use a wayland compositor as a desktop manager by having the compositor launch e.g. [gtkgreet](https://git.sr.ht/~kennylevinsen/gtkgreet) -l && wayland-logout.

![logout](/wayland-logout.png)
