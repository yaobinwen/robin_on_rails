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
