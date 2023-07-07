# Devicetree Specification

https://devicetree-specification.readthedocs.io/en/v0.3/introduction.html

1.1. Purpose and Scope

To initialize and boot a computer system, various software components interact. Firmware might perform low-level initialization of the system hardware before passing control to software such as an operating system, bootloader, or hypervisor. Bootloaders and hypervisors can, in turn, load and transfer control to operating systems. Standard, consistent interfaces and conventions facilitate the interactions between these software components. In this document the term boot program is used to generically refer to a software component that initializes the system state and executes another software component referred to as a client program. Examples of a boot program include: firmware, bootloaders, and hypervisors. Examples of a client program include: bootloaders, hypervisors, operating systems, and special purpose programs. A piece of software may be both a client program and a boot program (e.g. a hypervisor).

This specification, the Devicetree Specification (DTSpec), provides a complete boot program to client program interface definition, combined with minimum system requirements that facilitate the development of a wide variety of systems.

Other characteristics of embedded systems may include:

* a fixed set of I/O devices, possibly highly customized for the application
* a system board optimized for size and cost
* limited user interface
* resource constraints like limited memory and limited nonvolatile storage
* real-time constraints
* use of a wide variety of operating systems, including Linux, real-time operating systems, and custom or proprietary operating systems

1.4. Definition of Terms

https://devicetree-specification.readthedocs.io/en/v0.3/introduction.html#definition-of-terms

2.1. Overview

DTSpec specifies a construct called a devicetree to describe system hardware. A boot program loads a devicetree into a client program's memory and passes a pointer to the devicetree to the client.

- tree structure
- A node describes a device.
  - Has property/value pairs to describe the device characteristics.
  - Belongs to exactly one parent device. The root device has no parents.

A DTSpec-compliant devicetree describes device information in a system that **cannot necessarily be dynamically detected** by a client program. For example, the architecture of PCI enables a client to probe and detect attached devices, and thus devicetree nodes describing PCI devices might not be required. However, a device node is required to describe a PCI host bridge device in the system if it cannot be detected by probing.

2.2.1.1. Node Name Requirements

```
node-name@unit-address
```

The unit-address must match the first address specified in the reg property of the node. If the node has no reg property, the @unit-address must be omitted and the node-name alone differentiates the node from other nodes at the same level in the tree. The binding for a particular bus may specify additional, more specific requirements for the format of reg and the unit-address.

The root node does not have a node-name or unit-address. It is identified by a forward slash (/).
