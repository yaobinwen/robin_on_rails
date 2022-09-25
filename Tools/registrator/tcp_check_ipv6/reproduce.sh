#!/bin/sh

# This script reproduces the issue that `registrator` doesn't seem to form the
# TCP health check link appropriately for IPv6 addresses.
#
# Pre-conditions:
# 1). `docker` is installed.

# The IPv6 address that's used to register services.
IPv6_Addr="2601:547:980:b3e0::172f"

# Echo the commands and exit immediately on failure.
set -ex

# Start Consul.
# `boot2docker` is officially deprecated as of 2020-06-22.
# See: https://github.com/boot2docker/boot2docker
NUM=$(docker ps --filter "name=consul" --filter "status=running" --quiet | wc -l)
if [ $NUM -eq 0 ];
then
  docker run -d --name=consul --net=host consul
fi

# Make sure Consul is accessible.
curl http://localhost:8500/v1/catalog/services

# Pull in `registrator:master`.
docker pull gliderlabs/registrator:master

# Start `registrator`.
NUM=$(docker ps --filter "name=registrator" --filter "status=running" --quiet | wc -l)
if [ $NUM -eq 0 ];
then
  docker run -d \
    --name=registrator \
    --net=host \
    --volume=/var/run/docker.sock:/tmp/docker.sock \
    gliderlabs/registrator:master \
      -ip "${IPv6_Addr}" \
      consul://localhost:8500
fi

# Start a `redis` container for testing.
docker run -d \
  --env SERVICE_CHECK_TCP="true" \
  --env SERVICE_CHECK_INTERVAL="10s" \
  --env SERVICE_CHECK_TIMEOUT="1s" \
  --env SERVICE_ID="redis-service-id" \
  --env SERVICE_NAME="redis-service" \
  --name=redis \
  --publish-all \
  redis
