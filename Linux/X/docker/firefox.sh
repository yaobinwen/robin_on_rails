#!/bin/sh

set -e

: ${DISPLAY?not set}

X11_UNIX_SOCKET_FILE="/tmp/.X11-unix"
test -e "${X11_UNIX_SOCKET_FILE}" || {
  echo "'${X11_UNIX_SOCKET_FILE}' doesn't exist."
  exit 1
}

docker build -f Dockerfile.firefox -t firefox:0.0.1 .

xhost +

docker run -it --rm --name "ff-docker" -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix firefox:0.0.1

# Now, in another terminal window, run `docker exec -it ff-docker /bin/bash` to log in the container, then inside the container run `firefox` to open FireFox.

xhost -
