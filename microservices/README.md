# Microservices

## Overview

This folder has notes about microservices.

## Architecture

Refer to the architecture diagram in [1]:

![A typical service mesh architecture with data plane proxies deployed as sidecars and a separate control plane.](https://glasnostic.com/blog/microservices-architecture-patterns-service-mesh-glossary/service-mesh-architecture-ea486b17e8.png)

A **service mesh**:

- Is **NOT** a "mesh of services".
- Is "a mesh of **proxies** that services can plug into to completely abstract the network away."
- Has two key architectural components:
  - **data plane**
  - **control plane**

A **"sidecar"**:

- Is a software component.
- Is installed on the same machine next to the service (like a sidecar attached to a motorcycle, hence "sidecar").
- Acts as a proxy.
- Is called by the service so the service does not call the other services directly.
- Encapsulates the complexities of the service-to-service communication.

A **data plane** is the interconnected set of "sidecars".

A **control plane** is the set of APIs and tools that are used to control the behavior of the "sidecars".

An **API gateway**:

- Is a **reverse proxy** that "maps Microservices to APIs and exposes them to the outsides world."
- Acts as the intermediary between the clients and the microservices.
- Basic features include:
  - Authenticate requests.
  - Enforce security policies.
  - Load balance requests.
- See [3] for more details.

## References

- [1] [Microservices Glossary](https://glasnostic.com/blog/microservices-architecture-patterns-service-mesh-glossary)
- [2] [Managing Microservices with a Service Mesh: Data vs. Control Plane](https://glasnostic.com/blog/microservices-control-data-planes-differences-istio-linkerd)
- [3] [What is an API Gateway?](https://glasnostic.com/blog/what-is-an-api-gateway-aws-express-kong)
- [4] [The 2019 Microservices Ecosystem](https://glasnostic.com/blog/the-2019-microservices-ecosystem)
- [5] [A guide to open source for microservices](https://opensource.com/article/19/11/microservices-cheat-sheet)
