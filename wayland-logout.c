#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>


/* Man page says to #define _GNU_SOURCE to get ucred struct from sys/socket.h
 * but bits/socket.h actually checks for __USE_GNU here, so just define both */
#define _GNU_SOURCE
#define __USE_GNU

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
