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

## Chapter 02: Data Model and Query Languages

This chapter discusses two topics:
- The three major data models:
  - 1). relational
  - 2). non-relational
    - 2.1). document
    - 2.2). graph
- The query languages for each data model.

### Relational Model vs Document Model vs Graph Model

| Model | Pros | Cons | When to Use | Notes |
|:-----:|:-----|:-----|:------------|:------|
| Relational | 1. Good for `many-to-one/many` [1].<br>2. **Minimal duplication** when normalized.<br>3. `schema-on-write` ensures data validity early. | 1. Poor for `one-to-many`.<br>2. Schema change is expensive. | `many-to-one/many` is used to a certain extent [2]. ||
| Document | 1. Good for `one-to-many`.<br>2. Flexible schema due to `schema-on-read`. | 1. Poor for `many-to-one` or `many-to-many`.<br>2. Some **duplicated data** that adds maintenance burden.<br>3. `schema-on-read` doesn't gurantee data validity. | 1. Data mainly uses `one-to-many` relation and doesn't need much `join`.<br>2. Large parts of document are needed mostly. [3] ||
| Graph | 1. Good for `many-to-many`.<br>2. Good for `evolvability`. || `many-to-many` is common in the data. ||

Note [1]: Querying `many-to-one/many` relations requires `join` operations which are natively supported by relational databases, hence better performance. Document-based databases usually support `join` operations poorly so a lot of `join` must be performed by the application code, hence poorer performance.

Note [2]: When `many-to-many` relation is very common, consider **graph-like** data models.

Note [3]: When only a small portion of the document is needed, querying a large document is a waste. The book thus recommends "keep documents fairly small and avoid writes that increase the size of a document".

### Query Languages

The key point is: **A _declarative_ query language is better than an _imperative_ one.**

This section also covers the commonly used query languages used for different data models.

## Chapter 03: Storage and Retrieval

### Overview

Why should I care about how the database handles storage and retrieval internally? Because I do **need to select a storage engine** that is appropriate for the application.

Two considerations when selecting a storage engine:
- 1). Is this for **transactions**?
- 2). Is this for **analytics**?

Two families of storage engines:
- 1). `log-structured` storage engine.
- 2). `page-oriented` storage engine.

### Log-structured

A `log` is a generalized term that refers to **append-only sequence of records**. It can be text-based or binary-based.

For a log-structured storage engine, one needs to consider two questions:
- 1). A log-based storage engine usually **performs well on writes but poorly on reads**. How can we improve the read performance to have a good balance?
- 2). Records are appended to the log file so how can we avoid running out of disk space?

An `index` helps to improve the read performance, but it inevitably hurts the write performance. This is the trade-off.

### Hash Index

Suited situations:
- 1). The key space is reasonably large so it can be put in memory.
- 2). Writes are more frequent than reads. (This is because we are still discussing this topic in the context of log-structured storage engine so writes usually perform better than reads.)

### Compaction and Merge

One method to avoid running out of space is **compaction and merge**:
- Break the log into segments of a certain size by closing a segment file when it reaches a certain size, and making subsequent writes to a new segment file.
- We can then perform `compaction` on these segments. `Compaction` means throwing away duplicate keys in the log, and keeping only the most recent update for each key.

More issues to consider in real implementations:
- File format
- Deleting records
- Crash recovery
- Partially written records
- Concurrency control
