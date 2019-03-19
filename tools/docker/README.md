# Docker

## How to push a docker image?

1. Tag the docker image with the appropriate registry information.
  - `docker tag image_name:latest registry_host:1234/repo_name/image_name:latest`w
2. `docker push <image-name:tag>`.

## How to launch GUI application from within a docker container?

Refer to [Running GUI apps with Docker](http://fabiorehm.com/blog/2014/09/11/running-gui-apps-with-docker/) which uses this command: `docker run -d -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix <image_name:tag>`.

Also refer to [Running GUI Applications inside Docker Containers](https://medium.com/@SaravSun/running-gui-applications-inside-docker-containers-83d65c0db110) which uses this command: `docker run --network='host' --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" <image_name:tag>`.
