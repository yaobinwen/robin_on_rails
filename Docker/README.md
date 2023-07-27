# Docker

## How a docker image built? What is a "writable layer"?

See [4].

## How to push a docker image?

1. Tag the docker image with the appropriate registry information.
  - `docker tag image_name:latest registry_host:1234/repo_name/image_name:latest`w
2. `docker push <image-name:tag>`.

## How to launch GUI application from within a docker container?

Refer to [Running GUI apps with Docker](http://fabiorehm.com/blog/2014/09/11/running-gui-apps-with-docker/) which uses this command: `docker run -d -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix <image_name:tag>`.

Also refer to [Running GUI Applications inside Docker Containers](https://medium.com/@SaravSun/running-gui-applications-inside-docker-containers-83d65c0db110) which uses this command: `docker run --network='host' --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" <image_name:tag>`.

## How to generate core dump inside a container?

Simply setting `ulimit -c unlimited` inside the docker probably won't work. This issue [1] talks about this a lot. For example, the issue description says:

> On ubuntu the default setting - core files get piped to apport program. This is no good in containers. It must be overridden to something else if apport is not installed inside the container.
>
> It would also be a help if linux kernel could have a different value of /proc/sys/kern/pattern inside the container than outside it.

The words above explain why the core dump is not generated inside the container, and how it might be fixed.

I used the article [2] as a primary source which also refers to [1]. Eventually, here is how I generated core dumps inside the container:

- Change the `/proc/sys/kernel/core_pattern` file on the **host** machine: `echo '/tmp/core.%t.%e.%p' | sudo tee /proc/sys/kernel/core_pattern`. [2] says "Docker containers share the same `/proc` as the host." If so, I may be able to modify this file from within the container as long as I have the superuser privilege.
- Run the container with `--ulimit core=-1`: `docker run -d --name "my_container" --ulimit core=-1 image_name:tag`. [2] passes `--security-opt seccomp=unconfined` to `docker run`, but I ran the container without them and still succeeded.

I also tried the article [3], but didn't succeed. However, that may not be the article's fault. The key point was still the `/proc/sys/kernel/core_pattern` configuration. [3] requires the container be started with the superuser privilege so the `core_pattern` file can be modified inside the container, not outside as how [2] did it. Therefore, I guess (not verified) [3] wouldn't work for the containers that can't be started with superuser privilege.

## How to disable auto-restart on a container?

Run `docker update --restart=no my-container`.

See ["Restart policies (--restart)"](https://docs.docker.com/engine/reference/run/#restart-policies---restart) and [`docker update`](https://docs.docker.com/engine/reference/commandline/update/).

The container's configuration can be found at `/var/lib/docker/containers/<container-ID>/hostconfig.json`. It is in JSON format which can be pretty-print with `python3 -m json.tool < hostconfig.json`. Check for the `RestartPolicy.Name = "always"`.

## `docker login`

To use `docker login` without it complaining on a machine with no GUI, you have to build `docker-credential-pass` from [docker/docker-credential-helpers](https://github.com/docker/docker-credential-helpers), install it in `/usr/local/bin`, configure it in `~/.docker/config.json`, and then install `pass` and run `pass init`. `pass` assumes that you have GPG up and running with the GPG agent caching your passphrase.

Also you should not try to do the build on a machine with only 512MB of RAM or Bad Things™ will happen.

## Self-signed CA certificates

On the client side, when logging in a Docker registry that uses a self-signed CA certificate, `docker login` may report the error like

> x509: certificate signed by unknown authority

The document [_Verify repository client with certificates_](https://docs.docker.com/engine/security/certificates/) says:

> A custom certificate is configured by creating a directory under `/etc/docker/certs.d` using the same name as the registry's hostname, such as `localhost`. All `*.crt` files are added to this directory as CA roots.
>
> The presence of one or more `<filename>.key/cert` pairs indicates to Docker that there are custom certificates required for access to the desired repository.
>
>>Note: If multiple certificates exist, each is tried in alphabetical order. If there is a 4xx-level or 5xx-level authentication error, Docker continues to try with the next certificate.

An example configuration is as follows:

```
/etc/docker/certs.d/            <-- Certificate directory
    └── localhost:5000          <-- Hostname:port
       ├── client.cert          <-- Client certificate
       ├── client.key           <-- Client key
       └── ca.crt               <-- Root CA that signed the registry certificate, in PEM
```

I'm not sure if the CA certificate must be named as `ca.crt`. The words "All `*.crt` files are added to this directory as CA roots" seem to suggest that, if there are multiple CA certificates (as a chain), they can all be put under this folder with different filenames as long as their extensions are all `.crt`. In other words, maybe the following configuration also works:

```
/etc/docker/certs.d/            <-- Certificate directory
    └── localhost:5000          <-- Hostname:port
       ├── client.cert          <-- Client certificate
       ├── client.key           <-- Client key
       ├── root-ca.key          <-- Root CA certificate
       ├── intermediate-ca.key  <-- Intermediate CA certificate
       └── org-ca.crt           <-- Organization's CA that signed the registry certificate, in PEM
```

## How to list the contents in a registry?

Refer to [7]. Suppose the Docker Registry is running at `http://192.168.16.209:5000` (if it's running on `https`, `docker login` may be required). Then:

List repositories: `curl --silent --request GET http://192.168.16.209:5000/v2/_catalog | jq .`:

```json
{
  "repositories": [
    "ywen/image_1",
    "ywen/image_2"
  ]
}
```

List image tags: `curl --silent --request GET http://192.168.16.209:5000/v2/ywen/image_1/tags/list | jq .`:

```json
{
  "name": "ywen/image_1",
  "tags": [
    "tag1",
    "tag2",
    "tag3"
  ]
}
```

Get the manifests of `ywen/image_1:tag1`: `curl --silent --request GET http://192.168.16.209:5000/v2/ywen/image_1/manifests/tag1 | jq .`

## How to delete an image in a registry?

The registry must be configured to allow deleting images. This can be done in two ways:
- 1). Setting the environment variable `REGISTRY_STORAGE_DELETE_ENABLED=true` when starting the registry. For example, the Docker command can be:
```shell
docker run \
  --restart=always \
  --name=registry \
  -v /var/lib/registry:/var/lib/registry \
  -e REGISTRY_STORAGE_DELETE_ENABLED=true \
  registry:2
```
- 2). Or set it in the [registry's configuration](https://docs.docker.com/registry/configuration/#delete):
```yaml
delete:
  enabled: true
```

To delete an image, you must obtain the **digest** of the image. As noted in [7]:

> **Note** When deleting a manifest from a registry version 2.3 or later, the following header must be used when HEAD or GET-ing the manifest to obtain the correct digest to delete:
> ```
> Accept: application/vnd.docker.distribution.manifest.v2+json
> ```

To tell the version of the Docker registry, run `sudo docker exec -it registry-container-name registry --version`:

```
$ sudo docker exec -it registry registry --version
registry github.com/docker/distribution 2.8.2
```

It's version `2.8.2`, later than `2.3`, so I'll need to use the `Accept` header in the HTTP request. The full command is:

```
curl -I -sS -H "Accept: application/vnd.docker.distribution.manifest.v2+json" http://192.168.16.209:5000/v2/ywen/image_1/manifests/tag1
```

This command generates the output below:

```
HTTP/1.1 200 OK
Content-Length: 1776
Content-Type: application/vnd.docker.distribution.manifest.v2+json
Docker-Content-Digest: sha256:1bb74ab3a9f24b84bdd633b82dc2c500ff8b08cb60871b326f9a6588b6a9ae1c
Docker-Distribution-Api-Version: registry/2.0
Etag: "sha256:1bb74ab3a9f24b84bdd633b82dc2c500ff8b08cb60871b326f9a6588b6a9ae1c"
X-Content-Type-Options: nosniff
Date: Fri, 07 Jul 2023 17:52:16 GMT
```

To only print out the image digest, run the `curl` command through `awk`:

```
curl -I -sS -H "Accept: application/vnd.docker.distribution.manifest.v2+json" http://192.168.16.209:5000/v2/ywen/image_1/manifests/tag1 | awk '$1 == "Docker-Content-Digest:" { print $2 }'
```

This time it prints:

```
sha256:1bb74ab3a9f24b84bdd633b82dc2c500ff8b08cb60871b326f9a6588b6a9ae1c
```

Run `curl -v -sS -X DELETE "http://192.168.16.209:5000/v2/ywen/image_1/manifests/sha256:1bb74ab3a9f24b84bdd633b82dc2c500ff8b08cb60871b326f9a6588b6a9ae1c` to delete the image. Note `sha256:` must be included.

You should get `202 Accepted` in the response.

Then you should not see the images when you list them again `curl -sS http://localhost:5000/v2/mvs/health-monitor/tags/list | jq .` (or you will still see it but obtaining its manifest will return the error `MANIFEST_UNKNOWN` and I haven't learned why).

## How to do garbage collection in a registry?

Run `registry garbage-collect /etc/docker/registry/config.yml`. If the registry is a Docker container, run `sudo docker exec -it registry-container-name registry garbage-collect /etc/docker/registry/config.yml`. You may need to replace `/etc/docker/registry/config.yml` with the actual configuration path.

## References:
- [1] [Documentation: Make clear instructions for getting a core file, when container crashes](https://github.com/moby/moby/issues/11740)
- [2] [Core Dump File inside Docker](https://le.qun.ch/en/blog/core-dump-file-in-docker/)
- [3] [How to get core file of segmentation fault process in Docker](https://dev.to/mizutani/how-to-get-core-file-of-segmentation-fault-process-in-docker-22ii)
- [4] [Images and Layers](https://docs.docker.com/storage/storagedriver/#images-and-layers)
- [5] [Docker on Windows in Production](https://stackoverflow.com/q/45554311/630364)
- [6] [Kubernetes vs Docker Swarm — A Comprehensive Comparison](https://hackernoon.com/kubernetes-vs-docker-swarm-a-comprehensive-comparison-73058543771e)
- [7] [Docker Registry HTTP API V2](https://docs.docker.com/registry/spec/api/)
  - [Docker Registry HTTP API V2: Details](https://docs.docker.com/registry/spec/api/#detail)
- [8] [reg](https://github.com/genuinetools/reg): : Docker registry v2 command line client and repo listing generator with security checks.
