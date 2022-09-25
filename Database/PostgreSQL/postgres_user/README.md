# Issue: `Unknown user postgres`

## Overview

This folder reproduces the error `find: unknown user postgres`.

To reproduce the error, run `./reproduce.sh`. The provisioning Ansible playbook should error with the following output:

```
TASK [Create the database.] ****************************************************
task path: /home/ywen/yaobin/github/robin_on_rails/db/postgresql/postgres_user/ansible/reproduce.yml:47
Using module file /usr/lib/python2.7/dist-packages/ansible/modules/cloud/docker/docker_container.py
<127.0.0.1> Failed to connect to the host via ssh: 
fatal: [default]: FAILED! => {
    "changed": false, 
    "invocation": {
        "module_args": {
            "api_version": "auto", 
            "auto_remove": false, 
            "blkio_weight": null, 
            "ca_cert": null, 
            "cap_drop": null, 
            "capabilities": null, 
            "cleanup": true, 
            "client_cert": null, 
            "client_key": null, 
            "command": null, 
            "comparisons": null, 
            "cpu_period": null, 
            "cpu_quota": null, 
            "cpu_shares": null, 
            "cpuset_cpus": null, 
            "cpuset_mems": null, 
            "debug": false, 
            "detach": false, 
            "device_read_bps": null, 
            "device_read_iops": null, 
            "device_write_bps": null, 
            "device_write_iops": null, 
            "devices": null, 
            "dns_opts": null, 
            "dns_search_domains": null, 
            "dns_servers": null, 
            "docker_host": "unix://var/run/docker.sock", 
            "domainname": null, 
            "entrypoint": null, 
            "env": {
                "POSTGRES_PASSWORD": "my_pass"
            }, 
            "env_file": null, 
            "etc_hosts": null, 
            "exposed_ports": null, 
            "force_kill": false, 
            "groups": null, 
            "healthcheck": null, 
            "hostname": null, 
            "ignore_image": false, 
            "image": "postgres:9.6-alpine-initdb", 
            "init": false, 
            "interactive": false, 
            "ipc_mode": null, 
            "keep_volumes": false, 
            "kernel_memory": null, 
            "kill_signal": null, 
            "labels": null, 
            "links": null, 
            "log_driver": "journald", 
            "log_opt": {
                "tag": "postgresql-initdb"
            }, 
            "log_options": {
                "tag": "postgresql-initdb"
            }, 
            "mac_address": null, 
            "memory": "0", 
            "memory_reservation": null, 
            "memory_swap": null, 
            "memory_swappiness": null, 
            "mounts": null, 
            "name": "postgresql-9.6-alpine-initdb", 
            "network_mode": null, 
            "networks": null, 
            "networks_cli_compatible": null, 
            "oom_killer": null, 
            "oom_score_adj": null, 
            "output_logs": false, 
            "paused": false, 
            "pid_mode": null, 
            "pids_limit": null, 
            "privileged": false, 
            "published_ports": null, 
            "pull": false, 
            "purge_networks": false, 
            "read_only": false, 
            "recreate": false, 
            "restart": false, 
            "restart_policy": null, 
            "restart_retries": null, 
            "runtime": null, 
            "security_opts": null, 
            "shm_size": null, 
            "ssl_version": null, 
            "state": "started", 
            "stop_signal": null, 
            "stop_timeout": null, 
            "sysctls": null, 
            "timeout": 60, 
            "tls": false, 
            "tls_hostname": "localhost", 
            "tmpfs": null, 
            "trust_image_content": false, 
            "tty": false, 
            "ulimits": null, 
            "user": null, 
            "userns_mode": null, 
            "uts": null, 
            "validate_certs": false, 
            "volume_driver": null, 
            "volumes": [
                "/etc/passwd:/etc/passwd:ro", 
                "/var/lib/postgresql/data/db:/var/lib/postgresql/data"
            ], 
            "volumes_from": null, 
            "working_dir": null
        }
    }, 
    "msg": "find: unknown user postgres\n", 
    "status": 1
}
```

## Docker Image `postgres`

This reproduction uses the following materials:

- [`postgres`](https://hub.docker.com/_/postgres)
- Specifically, we use `postgres:9.6-alpine` which is created by this [`Dockerfile`](https://github.com/docker-library/postgres/blob/04bf35f0c4a3f7ac41591f9b28e2de1fecb7fef4/9.6/alpine/Dockerfile).
- Its [entrypoint script](https://github.com/docker-library/postgres/blob/04bf35f0c4a3f7ac41591f9b28e2de1fecb7fef4/9.6/alpine/docker-entrypoint.sh) can be found in the same directory.

Note that `docker-entrypoint.sh` is copied into `/usr/local/bin` and a symbolic link is created in the root path. The code is [here](https://github.com/docker-library/postgres/blob/04bf35f0c4a3f7ac41591f9b28e2de1fecb7fef4/9.6/alpine/Dockerfile#L144-L146):

```shell
COPY docker-entrypoint.sh /usr/local/bin/
RUN ln -s usr/local/bin/docker-entrypoint.sh / # backwards compat
ENTRYPOINT ["docker-entrypoint.sh"]
```

## Reproduce Using the Docker Image

When the `unknown user postgres` error happens, make sure you can find the following Docker images in the VM:

```
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
postgres            9.6-alpine-initdb   ad8f4b5ee62e        About an hour ago   37.6MB
postgres            9.6-alpine          4bb112d23234        6 days ago          37.6MB
```

Then run this command to start a container for debugging:

```
docker run -it --entrypoint "/bin/bash" --name "test_postgres" -v "/etc/passwd:/etc/passwd:ro" -v "/var/lib/postgresql/data/db:/var/lib/postgresql/data" -e POSTGRES_PASSWORD=mypass postgres:9.6-alpine-initdb; docker rm -f test_postgres
```

This command has two sub-commands:

- The first command starts a container.
- The second command removes the previously created container to allow to run the whole command repeatedly without having to remove the container manually.

Inside the container, you should see the prompt:

```
bash-5.0#
```

Make sure you are in `/`, the root directory, and run:

```shell
./docker-entrypoint.sh postgres
```

You should see the error message:

```
find: unknown user postgres
```

## Debug

Inside the `test_progres` container, install `vim` in order to modify the entrypoint script (note that `postgres` is built upon Alpine Linux so its package manager is `apk`):

```shell
bash-5.0# apk add vim
```

By adding `echo` at different places, we can find the `unknown user postgres` error happens at the `find` command in [these two lines](https://github.com/docker-library/postgres/blob/04bf35f0c4a3f7ac41591f9b28e2de1fecb7fef4/9.6/alpine/docker-entrypoint.sh#L56-L59):

```shell
  # allow the container to be started with `--user`
  if [ "$user" = '0' ]; then
    find "$PGDATA" \! -user postgres -exec chown postgres '{}' +
    find /var/run/postgresql \! -user postgres -exec chown postgres '{}' +
  fi
```

This is because when we started the Docker container, we mapped the host's `/etc/passwd` into the container. However, we didn't necessarily create the `postgres` user on the host system. As a result, when the host system doesn't have the `postgres` user, the `find` will report the `unknown user` error.

The `postgres` Docker image's `Dockerfile` actually [creates the `postgres` user](https://github.com/docker-library/postgres/blob/04bf35f0c4a3f7ac41591f9b28e2de1fecb7fef4/9.6/alpine/Dockerfile#L4-L10):

```dockerfile
# 70 is the standard uid/gid for "postgres" in Alpine
# https://git.alpinelinux.org/aports/tree/main/postgresql/postgresql.pre-install?h=3.12-stable
RUN set -eux; \
  addgroup -g 70 -S postgres; \
  adduser -u 70 -S -D -G postgres -H -h /var/lib/postgresql -s /bin/sh postgres; \
  mkdir -p /var/lib/postgresql; \
  chown -R postgres:postgres /var/lib/postgresql
```

## Possible Fixes

### Fix One: Create a `postgres` user on the host system

This fix is done with the following steps:

- On the host system: install `postgres` which automatically creates the user `postgres`.
- When creating the `postgres` container, map host's `/etc/passwd` into the container.

The pros:

- The container can initialize the database folder (`/var/lib/postgres`) using the same user model as the host system which may make it easier for the host system to access the data.

### Fix Two: Use the `postgres` user in the container

This fix is done with the following steps:

- Do not install `postgres` on the host system.
- Do not map host's `/etc/passwd` into the container.
- The `Dockerfile` creates a `postgres` user for the image `postgres:9.6-alpine-initdb` so it is used.

The pros:

- The host system has the minimal dependencies on `postgres`.

The cons:

- The container's user `postgres` is created with the ID `70` which is the default on Alpine Linux but may not be an appropriate ID for the other Linux distributions.
