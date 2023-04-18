# Cloud

## Overview

This folder has the notes about cloud computing.

## Review Questions

- What is an _application_ on the cloud?
- What is _service mesh_?
- What is a _data plane_?
- What is a _control plane_?
- How are _data plane_ and _control plane_ different from each other?

## Application

A cloud-based application is no longer a single executable program. It usually consists of multiple services.

## Service mesh

[1] uses the online shopping as an example to explain what a "service mesh" is:
- In a kind of abstract level, a service mesh is "a way to control how different parts of an application share data with one another."
- More specifically, a service mesh "routes requests from one service to the next, optimizing how all the moving parts work together."

Therefore, a service mesh:
- Knows the network topology of the services in the entire cloud-based application.
- Is typically implemented in the _sidecar_ pattern: a scalable set of network proxies deployed _alongside_ the services. Every service is **not aware of the network** and **only knows about its local proxy**.

One can understand a service mesh from two perspectives:
- Data plane
- Control plane

## Data plane vs. control plane

[2] explains these two terms very well.

A **data plane** focuses on the **"how to handle the data"** that the services request or receive. For example:
- Routing: When a service sends a request to the target service, where to find this "target service"?
- Load balancing: When a request arrives from another service, which service instance should be assigned to handle this request?
- Authentication & authorization: When a request arrives from another service, is this request allowed to invoke the requested service?

In effect, the **sidecar proxies** make up the data plane. As [2] summarizes, the data plane is responsible for "conditionally translating, forwarding, and observing every network packet that flows to and from a service instance."

A **control plane** focuses on **"teaching sidecar proxies how to handle the data"**. In other words, initially, the proxies are "pieces of blank paper" that know nothing about how to handle the data that flow through the services in the cloud-based application. The control plane "teaches" the proxies to know how to do that. In reality, this can be done in two ways:
- Via manual configuration by human beings.
- Via automatic configuration through some control plane software.

[2] explains them so well that I really want to quote the original words below.

A data plane performs the following tasks:

> - **Service discovery**: What are all of the upstream/backend service instances that are available?
> - **Health checking**: Are the upstream service instances returned by service discovery healthy and ready to accept network traffic? This may include both active (e.g., out-of-band pings to a /healthcheck endpoint) and passive (e.g., using 3 consecutive 5xx as an indication of an unhealthy state) health checking.
> - **Routing**: Given a REST request for /foo from the local service instance, to which upstream service cluster should the request be sent?
> - **Load balancing**: Once an upstream service cluster has been selected during routing, to which upstream service instance should the request be sent? With what timeout? With what circuit breaking settings? If the request fails should it be retried?
> - **Authentication and authorization**: For incoming requests, can the caller be cryptographically attested using mTLS or some other mechanism? If attested, is the caller allowed to invoke the requested endpoint or should an unauthenticated response be returned?
> - **Observability**: For each request, detailed statistics, logging, and distributed tracing data should be generated so that operators can understand distributed traffic flow and debug problems as they occur.

A control plane performs the following tasks:

- **The human**: There is still a (hopefully less grumpy) human in the loop making high level decisions about the overall system.
- **Control plane UI**: The human interacts with some type of UI to control the system. This might be a web portal, a CLI, or some other interface. Through the UI, the operator has access to global system configuration settings such as deploy control (blue/green and/or traffic shifting), authentication and authorization settings, route table specification (e.g., when service A requests /foo what happens), and load balancer settings (e.g., timeouts, retries, circuit breakers, etc.).
- **Workload scheduler**: Services are run on an infrastructure via some type of scheduling system (e.g., Kubernetes or Nomad). The scheduler is responsible for bootstrapping a service along with its sidecar proxy.
- **Service discovery**: As the scheduler starts and stops service instances it reports liveness state into a service discovery system.
- **Sidecar proxy configuration APIs**: The sidecar proxies dynamically fetch state from various system components in an eventually consistent way without operator involvement. The entire system composed of all currently running service instances and sidecar proxies eventually converge. Envoy’s universal data plane API is one such example of how this works in practice.

## Envoy Proxy

Envoy proxy is a data plane.

## References

- [1] [What's a service mesh?](https://www.redhat.com/en/topics/microservices/what-is-a-service-mesh)
- [2] [Service mesh data plane vs. control plane](https://blog.envoyproxy.io/service-mesh-data-plane-vs-control-plane-2774e720f7fc)
