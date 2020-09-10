/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Scott Moreau
 * Copyright (c) 2020 Érico Nogueira
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

// size of field sun_path in struct sockaddr_un
#define pathlen ((int)sizeof(((struct sockaddr_un *)NULL)->sun_path))

int main()
{
    char socket_path[pathlen];
    char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
    char *wayland_display = getenv("WAYLAND_DISPLAY");

    if (!wayland_display)
    {
        fprintf(stderr, "WAYLAND_DISPLAY not set\n");
        return -1;
    }

    // WAYLAND_DISPLAY can be an absolute path
    int wl_display_abs = wayland_display[0] == '/';

    if (!wl_display_abs && !xdg_runtime_dir)
    {
        fprintf(stderr, "WAYLAND_DISPLAY is not an absolute path and XDG_RUNTIME_DIR is not set\n");
        return -1;
    }

    if (wl_display_abs)
    {
        if (snprintf(socket_path, pathlen, "%s", wayland_display) >= pathlen)
        {
            fprintf(stderr, "WAYLAND_DISPLAY path \"%s\" is too long (max is %d)\n", wayland_display, pathlen);
            return -1;
        }
    } else
    {
        if (snprintf(socket_path, pathlen, "%s/%s", xdg_runtime_dir, wayland_display) >= pathlen)
        {
            fprintf(stderr, "XDG_RUNTIME_DIR/WAYLAND_DISPLAY path \"%s/%s\" is too long (max is %d)\n", xdg_runtime_dir, wayland_display, pathlen);
            return -1;
        }
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("Failed to create socket");
        return -1;
    }

    struct sockaddr_un socket = { 0 };
    socket.sun_family = AF_UNIX;
    // string length was checked in snprintf
    strcpy(socket.sun_path, socket_path);

    int ret = connect(fd, (struct sockaddr *)&socket, sizeof(socket));
    if (ret == -1)
    {
        fprintf(stderr, "Failed to connect to socket %s: %m\n", socket.sun_path);
        return -1;
    }

    struct ucred peer_cred;
    socklen_t optlen = sizeof(peer_cred);

    ret = getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &peer_cred, &optlen);
    if (ret == -1)
    {
        fprintf(stderr, "Error getting credentials for peer: %m\n");
        return -1;
    }

    ret = kill(peer_cred.pid, SIGINT);
    if (ret == -1)
    {
        fprintf(stderr, "Error killing pid %d: %m\n", peer_cred.pid);
        return -1;
    }

    return 0;
}
