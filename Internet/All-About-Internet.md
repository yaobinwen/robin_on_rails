# All About Internet

## 0. Questions & TODOs

- [ ] Unresolved questions in Appendix A [1]:
  - Page 14: "This raises a new question: what happens when the data to be sent is larger than the frame size?"
  - Page 16: "but this process raises an important question: How does a sending NIC know the MAC address of the NIC of which it's sending the data?" (See `network_demo` code where I want to program this process.)

## 1. Overview

## 2. Network Models

There are two prevailing network models: **OSI model** (7 layers) and **TCP/IP model** (4 layers):

<table border="1px solid black" border-collapse="collapse">
  <caption>Model Layer Comparison</caption>
  <tbody>
    <tr>
      <th>Layer</th>
      <th>OSI Model</th>
      <th>TCP/IP Model</th>
      <th>Protocol Data Unit (PDU)</th>
      <th>Function</th>
    </tr>
    <tr style="background:#d8ec9b;">
      <td>7</td>
      <td>Application</td>
      <td rowspan=3>Application</td>
      <td rowspan=3>Data (payload)</td>
      <td><p>High-level APIs, including resource sharing, remote file access</p></td>
    </tr>
    <tr style="background:#d8ec9b;">
      <td>6</td>
      <td>Presentation</td>
      <td><p>Translation of data between a networking service and an application; including character encoding, data compression and encryption/decryption</p></td>
    </tr>
    <tr style="background:#d8ec9b;">
      <td>5</td>
      <td>Session</td>
      <td><p>Managing communication sessions, i.e., continuous exchange of information in the form of multiple back-and-forth transmissions between two nodes</p></td>
    </tr>
    <tr style="background:#e7ed9c;">
      <td>4</td>
      <td>Transport</td>
      <td>Transport</td>
      <td>Segment, Datagram</td>
      <td><p>Reliable transmission of data segments between points on a network, including segmentation, acknowledgement and multiplexing</p></td>
    </tr>
    <tr style="background:#eddc9c;">
      <td>3</td>
      <td>Network</td>
      <td>Internet</td>
      <td>Packet</td>
      <td><p>Structuring and managing a multi-node network, including addressing, routing and traffic control</p></td>
    </tr>
    <tr style="background:#e9c189;">
      <td>2</td>
      <td>Data Link</td>
      <td rowspan=2>Link</td>
      <td>Frame</td>
      <td><p>Reliable transmission of data frames between two nodes connected by a physical layer</p></td>
    </tr>
    <tr style="background:#e9988a;">
      <td>1</td>
      <td>Physical</td>
      <td>Bit, Symbol</td>
      <td><p>Transmission and reception of raw bit streams over a physical medium</p></td>
    </tr>
  </tbody>
</table>

_References_:
- [1] [Wikipedia: Open Systems Interconnection model (OSI model)](https://en.wikipedia.org/wiki/OSI_model)
- [2] [Wikipedia: Internet protocol suite](https://en.wikipedia.org/wiki/Internet_protocol_suite)

## 3. Layer 1 & 2: Physical & Data Link

### 3.1 NIC

A **_network interface card (NIC)_** serves as the interface between the computer and the network.
- On older systems, a _NIC_ truly was a separate card that snapped into an expansion slot, which is why they were called network interface _cards_.
- They are still called NICs even though today they are built into the motherboard.

### 3.2 MAC Address

Every NIC has a unique identifier called **_media access control address_**, or **_MAC address_**.
- No two NICs share the same MAC address.
- IEEE is responsible in allocating the MAC addresses to different NIC manufacturers.
- A MAC address consists of 48 bits, or 12 hex digits:
  - The first 6 hex digits are _Organizationally Unique Identifier (OUI)_.
  - The last 6 hex digits are _device IDs_.
- Also called _physical address_.
- Also called _EUI-48_ (_Extended Unique Identifier_).

### 3.3 Frame

Data Link layer sends and receives data in **_frames_**. A frame consists of the following fields:
- Recipient's MAC address
- Sender's MAC address
- Type, or [`EtherType`](https://en.wikipedia.org/wiki/EtherType): indicates which protocol is encapsulated in the payload of the frame so the receiving end knows how to process the data. See [EtherType:Values](https://en.wikipedia.org/wiki/EtherType#Values) for possible values.

### 3.4 Repeater, Bridge, Hub, Switch, Router, Gateway

See Page 14-15 in [A.1] and the introduction section of Chapter 14 in [A.2]:

A **_repeater_** makes an except copy of the receiving frame and send a copy of the original frame out of all the connected ports except the port on which the frame originates. _Every_ frame sent on a network is received by _every_ NIC, but **only the NIC with the matching MAC address will process the frame**.
- A repeater does not have any knowledge of protocols. It simply copies data bit by bit.
- A repeater that has multiple ports is called a **_hub_**.

A **_bridge_** understands **link layer** protocols and therefore copies data frame by frame, instead of bit by bit. It does not send all incoming frames to all network devices that are connected to it, but sends the frame **only to the interface associated with the destination MAC address**.
- A _bridge_ is also called a **_switch_**.
- The term _bridge_ is mainly used in the documentation (such as the IEEE specifications).
- The term _switch_ is mainly used when people talk about and refer to the device. However, when a _switch_ has only **two ports**, people may also use the term _bridge_ to refer to it.

A **_router_** understands L3 network protocols such as IP, and forwards ingress packets based on a routing table. The term **_gateway_**, which was introduced before router, is also commonly used to refer to a router. But they are not exactly the same:
- They both help forward data between networks.
- However, a router routes data between the networks based on the same protocol, while a gateway routes data between dissimilar networks so a gateway needs to **translate from one protocol to another**.

## Appendix A: Common References

- [1] [CompTIA Network+ Certification All-in-One Exam Guide, Seventh Edition (Exam N10-007)](https://www.amazon.com/CompTIA-Network-Certification-Seventh-N10-007/dp/1260122387/)
- [2] [Understanding Linux Network Internals](https://www.amazon.com/Understanding-Linux-Network-Internals-Networking/dp/0596002556)

## Appendix B: Tools

- [1] [IP Subnet Calculator](https://www.calculator.net/ip-subnet-calculator.html)
