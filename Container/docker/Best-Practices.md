# Best Practices

## Writing Dockerfile

Tip #1: Order matters for caching: Order your steps from least to most frequently changing steps to optimize caching.

Tip #2: More specific COPY to limit cache busts: Only copy what’s needed. If possible, avoid “COPY  .” When copying files into your image, make sure you are very specific about what you want to copy.

Tip #3: Identify cacheable units such as apt-get update & install: When installing packages from package managers, you always want to update the index and install packages in the same RUN: they form together one cacheable unit. Otherwise you risk installing outdated packages.

Tip #4: Remove unnecessary dependencies: Remove unnecessary dependencies and do not install debugging tools. If needed debugging tools can always be installed later.

Tip #5: Remove package manager cache: Remove the cache in the same RUN instruction that installed packages. Removing it in another RUN instruction would not reduce the image size.

Tip #6: Use official images when possible. (For images of smaller size and better security.)

Tip #7: Use more specific tags: Do not use the latest tag. It has the convenience of always being available for official images on Docker Hub but there can be breaking changes over time.

Tip #8: Look for minimal flavors: The slim variant is based on a stripped down Debian, while the alpine variant is based on the even smaller Alpine Linux distribution image. A notable difference is that debian still uses GNU `libc` while alpine uses `musl libc` which, although much smaller, may in some cases cause compatibility issues.

Tip #9: Build from source in a consistent environment (which is a Docker container).

Tip #10: Fetch dependencies in a separate step.

Tip #11: Use multi-stage builds to remove build dependencies (recommended Dockerfile).

## Security

Keep the image as small as possible in order to reduce the attack surface. Possible approaches include:
- Avoid unnecessary packages.
- Prefer minimalist base images.
- Use multi-stage builds if you compile code in a container (so the final image is small).

Build images to do just one thing.

Use images from trustworthy sources:
- Use official, purpose-built images.
- Look at the Dockerfiles if you need to use an unofficial Docker image (which also lets you build the image yourself instead of using the unofficial image).

Permission management:
- Just enough permissions.
- Don't run containers in an elevated mode. Try hard to avoid using `–privileged` and `–cap-add`.
- Use image signing (Docker Content Trust).

Use application gateways and firewalls. As with any application, it’s always best to place an application behind a security-hardened system that can scan traffic coming into an application for malicious content.

## Others

Persist data outside of a container.

Treat containers as immutable.

## References

- [1] [Docker development best practices](https://docs.docker.com/develop/dev-best-practices/)
- [2] [Best practices for writing Dockerfiles](https://docs.docker.com/develop/develop-images/dockerfile_best-practices/)
- [3] [Image-building best practices](https://docs.docker.com/get-started/09_image_best/)
- [4] [Docker Blog: Intro Guide to Dockerfile Best Practices](https://www.docker.com/blog/intro-guide-to-dockerfile-best-practices/)
- [5] [Docker Blog: Speed Up Your Development Flow With These Dockerfile Best Practices](https://www.docker.com/blog/speed-up-your-development-flow-with-these-dockerfile-best-practices/)
- [6] [Security Best Practices for Docker Images](https://www.wintellect.com/security-best-practices-for-docker-images/)
