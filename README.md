# wayland-logout

wayland-logout is a simple program that sends SIGINT signal to a wayland compositor by looking up the pid for the wayland socket file. The path to the socket file is derived from WAYLAND_DISPLAY and XDG_RUNTIME_DIR environment variables. There are C and python implementations in this repo. Use meson to build an executable from the source. The python script is standalone. These can be used to kill a specific compositor instance and can be used in programs such as [wlogout](https://github.com/ArtsyMacaw/wlogout). This is a linux-specific implementation and will not work on e.g. FreeBSD currently, though PRs are welcome to make it work there.

![logout](/wayland-logout.png)
