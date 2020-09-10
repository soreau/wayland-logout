#!/usr/bin/python3

# Copyright 2020 Scott Moreau

import socket
import signal
import struct
import os

xdg_runtime_dir = os.getenv('XDG_RUNTIME_DIR')
wayland_display = os.getenv('WAYLAND_DISPLAY')

if not wayland_display:
    print('WAYLAND_DISPLAY not set')
    exit()

wayland_display_abs_path = wayland_display[0] == '/'

if not wayland_display_abs_path and not xdg_runtime_dir:
    print('WAYLAND_DISPLAY is not an absolute path and XDG_RUNTIME_DIR not set')
    exit()

if wayland_display_abs_path:
    socket_path = wayland_display
else:
    socket_path = xdg_runtime_dir + '/' + wayland_display

s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
s.connect(socket_path)
data = s.getsockopt(socket.SOL_SOCKET, socket.SO_PEERCRED, 12)
st = struct.Struct('I I I')
peer_cred = st.unpack(data)
os.kill(peer_cred[0], signal.SIGINT)
