# Docker Content Trust

## Overview

Docker Content Trust (DCT) is essentially the technology that **signs the Docker image**.

## Key Points

(Most of the following content is quoted from [1].)

> An individual image record has the following identifier:
>
> ```
> [REGISTRY_HOST[:REGISTRY_PORT]/]REPOSITORY[:TAG]
> ```
>
> A particular image `REPOSITORY` can have multiple tags. DCT is associated with the `TAG` portion of an image.
>
> An image repository can contain an image with one tag that is signed and another tag that is not.
>
> If a consumer enables DCT, they can only pull, run, or build with trusted images. Enabling DCT is a bit like applying a "filter" to your registry. Consumers "see" only signed image tags and the less desirable, unsigned image tags are "invisible" to them.

## DCT Keys

Because DCT is all about signing, it must involve keys. DCT uses the following keys:


- An `offline key` that is the root of DCT for an image tag.
- Repository or `tagging keys` that sign tags.
- Server-managed keys such as the `timestamp key`, which provides freshness security guarantees for your repository

<img alt="DCT Keys Illustration" src="https://docs.docker.com/engine/security/trust/images/trust_components.png" height="70%" width="70%" />

In the section "Signing Images with Docker Content Trust", the document starts to talk about a "delegation key pair" (public and private). I don't know why they don't keep with the existing terms, and I'm kind of bothered by this because the inconsistency makes me unable to see the relationship between the "delegation key pair" and the earlier mentioned three keys. But my current understanding is:

- The `offline key` is the private key because:
  - It is "offline" which means it's stored on the publisher's local machine. This indicates the confidentiality of this key so it's likely to be the private key.
  - Also, the `offline key` is used to create the tagging keys (see below), so it matches the role of the private key.
- The `tagging key` should be the public key, because the document says "we will need to add the delegation public key to the Notary server". The document doesn't how the "tagging key" will be used, but I guess they are associated with the repository so the end users can fetch them and use them to verify if the pulled images are indeed signed by the claimed publisher.
- I'm not sure about the `timestamp key` yet.

## Questions

- Is `tagging key` the signing private sub-key? Or is it the public key?
- How does DCT prevent the malicious user from hacking the entire repository so the malicious user can replace the `tagging key`?

## References

- [1] [Content trust in Docker](https://docs.docker.com/engine/security/trust/)
