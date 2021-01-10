# Designing Data-Intensive Applications

## Chapter 01: Reliable, Scalable, and Maintainable Applications

This chapter discusses the three most important concerns in most data-intensive systems:
- **Reliability**
- **Scalability**
- **Maintainability**

### Reliability

`Reliability` roughly means "continuing to work correctly, even when things go wrong."

`fault` is usually defined as one component of the system deviating from its spec. It is different from _failures_ which may cause the entire system as a whole stops working correctly.

`fault-tolerant` is **NOT** for all kinds of faults; it is only for **certain types of faults**.

**Tolerating faults is usually preferred over preventing faults** unless the faults cannot be cured (such as security faults).

The three kinds of faults that can be cured:
- Hardware Faults: How to cure:
  - Now we use software fault-tolerance techniques to allow single node failure without causing the failure of the entire system.
- Software Errors: How to cure:
  - Verify the assumptions and interactions in the system.
  - Test thoroughly.
  - Isolate processes.
  - Allow processes to crash and restart.
  - Measure, monitor, and analyze system behaviors in production.
  - Do self-check and alert if a discrepency is found.
- Human Errors: How to cure:
  - Design systems in a way that minimizes opportunities for error.
  - Decouple the places where people make the most mistakes from the places where they can cause failures.
  - Test thoroughly at all levels, from unit tests to whole-system integration tests and manual tests.
  - Allow quick and easy recovery from human errors, to minimize the impact in the case of a failure.
  - Set up detailed and clear monitoring.
  - Implement good management practices and training.

### Scalability

Two things to do before devising a plan to handle scalability:
- **Describe load**: Figure out the right `load parameters` to describe the system load.
- **Describe performance**:
  - The data in performance can help answer the two questions:
    - When you increase a load parameter and keep the system resources (CPU, memory, network bandwidth, etc.) unchanged, how is the performance of your system affected?
    - When you increase a load parameter, how much do you need to increase the resources if you want to keep performance unchanged?
  - `percentiles` and `median` may be more accurate to reflect the performance.

The approaches:
- Scaling up vs scaling out.
- Automatic scaling vs manual scaling.
- Stateless vs stateful.

### Maintainability

Consider the following three aspects:
- Operability
  - Monitoring the health of the system and quickly restoring service if it goes into a bad state
  - Tracking down the cause of problems, such as system failures or degraded performance
  - Keeping software and platforms up to date, including security patches
  - Keeping tabs on how different systems affect each other, so that a problematic change can be avoided before it causes damage
  - Anticipating future problems and solving them before they occur (e.g., capacity planning)
  - Establishing good practices and tools for deployment, configuration management, and more
  - Performing complex maintenance tasks, such as moving an application from one platform to another
  - Maintaining the security of the system as configuration changes are made
  - Defining processes that make operations predictable and help keep the production environment stable
  - Preserving the organization’s knowledge about the system, even as individual people come and go
- Simplicity
- Evolvability

