#!/bin/sh

# REQUIREMENTS - lsof (not always present)

if [ -z "${WAYLAND_DISPLAY}" ]; then
	printf '%s\n' 'Error: WAYLAND_DISPLAY not set'
	exit 1
fi

case "${WAYLAND_DISPLAY}" in
	/*)
		SOCKET_PATH="${WAYLAND_DISPLAY}"
		;;
	*)
		if [ -z "${XDG_RUNTIME_DIR}" ]; then
			printf '%s\n' \
				'Error: WAYLAND_DISPLAY is not an absolute path and XDG_RUNTIME_DIR not set'
			exit 2
		fi
		SOCKET_PATH="${XDG_RUNTIME_DIR}/${WAYLAND_DISPLAY}"
		;;
esac

WAYLAND_PIDS="$(lsof -t -f -- ${SOCKET_PATH})"
set -- $WAYLAND_PIDS
if [ $# -gt 1 ]; then
	printf '%s\n' 'Error: More than one process has been bound to the socket'
	exit 3
else
	kill -INT $1
fi
