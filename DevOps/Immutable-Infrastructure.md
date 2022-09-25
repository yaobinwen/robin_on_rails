# Immutable Infrastructure

## Definition: Mutable vs Immutable Paradigm

The article [1] explains the advantages of immutable infrastructure well.

In the **"mutable infrastructure" paradigm**, a server is "upgraded" from version 1 to version 2 by running a set of instructions. Because any instruction may fail, this introduces additional states between version 1 and version 2. However:

- Usually, only the version 1 and version 2 are well tested. The development team has no knowledge of whether a transient state works well or not. This may impact the user's experience of the service and bring risk to the system.
- Secondly, the additional in-between states introduced by mutating a server introduces unnecessary complexity because instead of managing only two well-defined states, the development team must handle much more states of the servers.
- Think about the case when the team must manage 1000 servers, with each server possibly landing in any of the in-between states. Even if the success rate of an individual server is very high, the success rate of all the 1000 servers is in fact much lower than expected. See notes [1].
- Even if the probability of success is high on one server, when there are many servers, the probability of success of all the servers is still low enough to be cautious.

In the **"immutable infrastructure" paradigm**, the team only need to deal with two well-defined states. The deployment can be done in the "blue-green" deployment model, which can guarantee a smooth transition of the user traffic from the earlier version of the service to the newer version.

## Benefits

Many articles have talked about the benefits of immutable infrastructure (e.g., [2] and [3]). But after reading many of them, I think the key benefits can be summarized as follows (which are also mentioned in the "Conclusion" section in [4]):

- **Repeatable**: Because the state of a piece of immutable infrastructure is clearly defined (via Infrastructure-as-Code), we can easily recreate the state of the infrastructure of a previous deployment.
- **Predictable**: Because the state of a piece of immutable infrastructure is clearly defined, we can easily foresee what will and will not happen with sufficient testing on functional or non-functional behaviors (such as performance and security). For example:
  - It prevents configuration drift [5] so every server, no matter how many there are in total, has exactly the same configuration with each other. When we refer to a particular machine, we are 100% certain about its configuration.
  - If we review the infrastructure's specification and find it lacks a certain piece of security software, we will be sure that the infrastructure is exposed to the corresponding security risks. We can then take further measures to remediate the problem.
- **Undoable**: With the help of virtualization (specifically, creating a snapshot of the system or committing the system's current state into an image), we can easily roll back to an earlier state of the system if the new deployment is problematic.

## How to Implement Immutable Infrastructure

The article [6] lists the strongly recommended elements to implement immutable infrastructure:

- Servers in a cloud computing environment, or another virtualized environment (like containers). "The key here is to have isolated instances with fast provisioning from custom images, as well as automated management for creation and destruction via an API or similar."
- "Full automation of your entire deployment pipeline, ideally including post-creation image validation."
- A [service-oriented architecture](https://en.wikipedia.org/wiki/Service-oriented_architecture).
- "A stateless, volatile application layer which includes your immutable servers. Anything here can get destroyed and rebuilt quickly at any time (volatile) without any loss of data (stateless)."
- A persistent data layer that includes:
  - Centralized logging
  - External data stores
- Dedication from engineering and operations teams to collaborate and commit to the approach.

## Notes

[1] Suppose the success rate of mutable upgrade on an individual server is `99.99%`, then the rate of "all success without any failure on N servers" is `0.9999^N`. We have the following statistics:

| N | Total Success Rate |
|:-:|:------------------:|
| 1 | 99.99% |
| 10 | 99.900044988% |
| 50 | 99.501223% |
| 100 | 99.0049339% |
| 500 | 95.1227046% |
| 1000 | 90.4832894% |

If the success rate of an individual server drops to `99.9%`, the table of statistics will look like the following:

| N | Total Success Rate |
|:-:|:------------------:|
| 1 | 99.9% |
| 10 | 99.00448802% |
| 50 | 95.12056282% |
| 100 | 90.47921471% |
| 500 | 60.63789449% |
| 1000 | 36.76954248% |

If the success rate of an individual server drops to `99%`, the table of statistics will look like the following:

| N | Total Success Rate |
|:-:|:------------------:|
| 1 | 99% |
| 10 | 90.4382075% |
| 50 | 60.50060671% |
| 100 | 36.60323413% |
| 500 | 0.6570483042% |
| 1000 | 0.004317124741% |

## References

- [1] [What is Mutable vs. Immutable Infrastructure?](https://www.hashicorp.com/resources/what-is-mutable-vs-immutable-infrastructure/)
- [2] [The Benefits of Immutable Infrastructure](https://eplexity.com/blog/benefits-of-immutable-infrastructure/)
- [3] [The Benefits of Immutable Infrastructure](https://anexinet.com/blog/the-benefits-of-immutable-infrastructure/)
- [4] [Incremental deployment vs full deployment](https://xebia.com/blog/incremental-deployments-vs-full-redeployments/)
- [5] [Configuration Drift](http://kief.com/configuration-drift.html)
- [6] [What Is Immutable Infrastructure?](https://www.digitalocean.com/community/tutorials/what-is-immutable-infrastructure)
