# Envoy Proxy

## Overview

This document is the notes of reading [Envoy Proxy](https://www.envoyproxy.io/docs/envoy/latest/) documentation.

## Introduction

### What is Envoy

Envoy is:

- an L7 proxy and
- a communication bus

The design goals are:

- The network should be transparent to applications.
- Easy to determine the source of the problem.

Envoy provides the following high level features to achieve the goals.

### High Level Features

- **Out of process architecture**
  - An Envoy process is **self-contained** and runs outside other processes.
  - All the Envoy processes form a **transparent communication mesh**.
  - "each application sends and receives messages **to and from localhost** and is **unaware of the network topology**"
  - Benifits (vs traditional library approach):
    - 1). Envoy works with any programming languages.
    - 2). Upgrade is simple.
- **Avoid messing up with the "tail latency" issue**
  - The document calls the feature "Modern C++11 code base" which really isn't a feature.
  - The key points of using C++11 are:
    - Tail latency is usually caused by the use of programming languages that are development-productive but not performing well, such as PHP, Python, Ruby.
    - C++11 produces native code which usually performs well; compared to C, C++11 provides better development productiviy.
- **L3/L4 filter architecture** + **HTTP L7 filter architecture**
  - Using a `filter` chain mechanism.
  - The filters can work with either L3/L4 (TCP/IP) or L7 (HTTP)
- **Support of both HTTP/1.1 and HTTP/2 bidirectionally**
  - "This means that any combination of HTTP/1.1 and HTTP/2 clients and target servers can be bridged."
- **HTTP L7 routing**
  - Rerouting/redirecting HTTP requests based on path, authority, content type, etc..
- **gRPC support**
- **MongoDB L7 support**
- **DynamoDB L7 support**
- **Service discovery and dynamic configuration**
- **Health checking**
- **Advanced load balancing**
- **Front/edge proxy support**
  - Can be used at the edge (observability, management, identical service discovery and load balancing algorithms, etc.).
- **Best in class observability**
