# HAProxy

## Overview

This document has my notes about [`HAProxy`](http://www.haproxy.org/). As of March 4, 2019, I'm using HAProxy 1.8.

## Introduction

The article [1] gives a good introduction of the fundamental concepts.

HAProxy is a "TCP/HTTP Load Balancer and proxying solution".

`Access Control List (ACL)` is used to define the condition that's used to redirect traffic. It is defined in the form of `acl <name> <condition>`. For example: `acl url_blog path_beg /blog`, where `acl` is the keyword, `url_blog` the name, and `path_beg /blog` the condition.

[2] has `ACL`'s detailed usage.

A `backend` is **a set of servers** that receives forwarded requests. In its most basic form, a backend has two parts:

- its load balance algorithm
- a list of servers

For example:

```
backend blog-backend
   balance roundrobin
   mode http
   server blog1 blog1.yourdomain.com:80 check
   server blog1 blog1.yourdomain.com:80 check
```

A `frontend` defines how requests should be forwarded to backends. Typicall, it has three components:

- The `bind`(See [3]) rule that defines the listening addresses and/or ports for the incoming traffic.
- The ACL rules.
- `use_backend`(See [4]) rules that match the ACL rules to the backends that deal with the traffic.

See [5] for HAProxy's `frontend` and `backend` configuration.

There are three **types of load balancing**:

- No load balancing.
- Layer 4 load balancing (`mode`(See [6]) defined as `tcp`).
- Layer 7 load balancing (`mode` defined as `http`).

The commonly used **load balancing algorithms** (see [7] for a complete list) include:

- roundrobin
- leastconn: 
- source: This selects which server to use based on a hash of the source IP i.e. your user's IP address. This method ensures that a user will connect to the same server.

The post [8] explains `sticky session` (aka `session affinity`) concisely. Basically, the sticky session feature makes sure the requests from the same user is handled by the same server. The article [9] illustrates this concept. [10] explains what a session is and why a sticky session is needed and can improve the application. Basically, a session remembers the conversational context between the client and server. Load balancing may forward the requests from the same client to different servers, but the servers may not share the session. Therefore, as [8] explains, the servers either start to share the session (which is not desired because that adds technological complexity), or implement _sticky session feature_ so the same client is always handled by the same server.

> That’s what sticky sessions are: the load balancer routing the same client to the same application server. And that’s why they’re so important: If the load balancer supports sticky sessions then you don’t need to modify your application to remember client session context.

> In most of the cases, the session will be lost. In case of [AWS ESB](https://docs.aws.amazon.com/elasticloadbalancing/latest/classic/elb-sticky-sessions.html): _If an instance fails or becomes unhealthy, the load balancer stops routing request to that instance, instead chooses a new healthy instance based on the existing load balancing algorithm. The load balancer treats the session as now "stuck" to the new healthy instance, and continues routing requests to that instance even if the failed instance comes back._

## References

- [1] [An Introduction to HAProxy and Load Balancing Concepts](https://www.digitalocean.com/community/tutorials/an-introduction-to-haproxy-and-load-balancing-concepts)
- [2] [Keyword: acl](http://cbonte.github.io/haproxy-dconv/1.8/configuration.html#4-acl)
- [3] [Keyword: bind](http://cbonte.github.io/haproxy-dconv/1.8/configuration.html#4-bind)
- [4] [Keyword: use_backend](http://cbonte.github.io/haproxy-dconv/1.8/configuration.html#4-use_backend)
- [5] [HAProxy configuration: Proxy](http://cbonte.github.io/haproxy-dconv/1.8/configuration.html#4)
- [6] [Keyword: mode](http://cbonte.github.io/haproxy-dconv/1.8/configuration.html#4-mode)
- [7] [Keyword: balance](http://cbonte.github.io/haproxy-dconv/1.8/snapshot/configuration.html#4-balance)
- [8] [Sticky and NON-Sticky sessions](https://stackoverflow.com/a/13641836/630364)
- [9] [New Elastic Load Balancing Feature: Sticky Sessions](https://aws.amazon.com/blogs/aws/new-elastic-load-balancing-feature-sticky-sessions/)
- [10] [Elastic Load Balancing with Sticky Sessions](https://shlomoswidler.com/2010/04/elastic-load-balancing-with-sticky-sessions.html)
