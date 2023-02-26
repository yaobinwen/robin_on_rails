# Envoy

## Terminology

A **host**:
- Is an **application**.
  - Can be a regular program/application.
  - Can be another Envoy proxy.
- Can do **network communication**.
- Has its own **network address**.
- A (physical or virtual) machine can contain one or more hosts.

A **downstream host**:
- Connects to Envoy.
- **Sends** a request.
- **Receives** a response.

An **upstream host**:
- Connects to Envoy.
- **Receives** a request.
- **Sends** a response.

A **listener**:
- Is a piece of software module.
- Is associated with a named network address (e.g., port, unix domain socket).
- Listens to incoming requests from the downstream hosts.
- Can be connected by the downstream hosts.

A **cluster**:
- Is a group of **upstream** hosts.
- These upstream hosts are **logically similar** so they can provide interchangeable services.
- Its members are discovered via _service discovery_.
- Its health is checked by _active health checking_.
- Envoy uses load balance policy to decide which member to route a request to.

> **Mesh**: A group of hosts that coordinate to provide a consistent network topology. In this documentation, an "Envoy mesh" is a group of Envoy proxies that form a message passing substrate for a distributed system comprised of many different services and application platforms.

> **Runtime configuration**: Out of band realtime configuration system deployed alongside Envoy. Configuration settings can be altered that will affect operation without needing to restart Envoy or change the primary configuration.

## Threading Model

Envoy uses a **single process with multiple threads**:
- One **coordination** thread (as the primary thread).
- A number of **worker** threads that do actual work: listening, filtering, forwarding.

> Once a connection is accepted by a listener, the connection spends the rest of its lifetime bound to a single worker thread. This allows the majority of Envoy to be **largely single threaded**.

> all worker threads independently attempt to accept connections on each listener and rely on the kernel to perform adequate balancing between threads.

## Listeners

Envoy configuration supports any number of listeners within a single process.

Generally we recommend running **a single Envoy per machine** regardless of the number of configured listeners.
