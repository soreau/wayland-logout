/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Scott Moreau
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

int main()
{
    char socket_path[108];
    char *xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
    char *wayland_display = getenv("WAYLAND_DISPLAY");

    if (!xdg_runtime_dir)
    {
        printf("XDG_RUNTIME_DIR not set\n");
        return -1;
    }
    if (!wayland_display)
    {
        printf("WAYLAND_DISPLAY not set\n");
        return -1;
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1)
    {
        printf("Failed to create socket: %s\n", strerror(errno));
        return -1;
    }
    
    sprintf(socket_path, "%s/%s", xdg_runtime_dir, wayland_display);

    struct sockaddr_un socket;
    memset(&socket, 0, sizeof(socket));
    socket.sun_family = AF_UNIX;
    strncpy(socket.sun_path, socket_path, sizeof(socket.sun_path) - 1);

    int ret = connect(fd, (struct sockaddr *)&socket, sizeof(socket));
    if (ret == -1)
    {
        printf("Failed to bind socket: %s\n", strerror(errno));
        return -1;
    }

    struct ucred peer_cred;
    socklen_t optlen = sizeof(struct ucred);

    ret = getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &peer_cred, &optlen);
    if (ret == -1)
    {
        printf("Error getting credentials for peer: %s\n", strerror(errno));
        return -1;
    }

    ret = kill(peer_cred.pid, SIGINT);
    if (ret == -1)
    {
        printf("Error killing pid %d: %s\n", peer_cred.pid, strerror(errno));
        return -1;
    }

    return 0;
}
