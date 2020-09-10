# wayland-logout

wayland-logout is a simple program that sends SIGINT signal to a wayfire compositor by looking up the pid for the wayland socket file. The socket file is derived from WAYLAND_DISPLAY and XDG_RUNTIME_DIR environment variables. This is a linux-specific implementation and will not work on e.g. FreeBSD.

![logout](/wayland-logout.png)
