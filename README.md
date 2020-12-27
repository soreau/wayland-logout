![logout](/wayland-logout.png)

# Wayland Logout

Wayland Logout is a utility designed to kill any wayland compositor that uses libwayland-server. It looks up the PID for the socket file by checking the socket path environment variables and sends a SIGTERM signal. This is useful as a way to logout of a wayland compositor, as the name implies.

### Installing

```
$ meson build
$ ninja -C build
# ninja -C build install
```

### Example usage
To end the wayland compositor session:
```
wayland-logout
```
To end a particular compositor instance by socket file:
```
WAYLAND_DISPLAY=/run/user/1000/wayland-2 wayland-logout
```
To have the compositor exit after a running client completes. This is useful for using the compositor as a desktop manager:
```
gtkgreet -l && wayland-logout
```

### License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
