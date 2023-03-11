#!/bin/sh

PY_FILE="$1"

docker run \
  -it --rm \
  --name my-running-script \
  -v "$PWD":/usr/src/myapp \
  -w /usr/src/myapp \
  python:3.11 \
  python "$PY_FILE"
