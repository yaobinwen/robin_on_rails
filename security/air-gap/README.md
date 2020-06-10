# Air Gap (网闸)

## Overview

Key notes about an "air gap" (or "air wall", or "air gapping"):

- It is a network security measure.
- The protected system is either:
  - phsycially isolated from the insecure network (not plugged into any wired or wireless network) or
  - no logical connection is automated ("data is transferred through the interface only manually, under human control" [2])
- Data is written into a physical medium (e.g., a thumbdrive) and physically moved between systems by personnel.
- In some environment:
  - Systems are classified as "low side" (or "black") and "high side" (or "red").
  - Data from low-to-high can be applied with minimal security measures, while high-to-low "requires much more stringent procedures to ensure protection of the data at a higher level of classification" [1].

## References

- [1] [Air gap (networking)](https://en.wikipedia.org/wiki/Air_gap_(networking))
- [2] [RFC 4949](https://tools.ietf.org/html/rfc4949)
- [3] [Recommended Practice: Improving Industrial Control System Cybersecurity with Defense-in-Depth Strategies](https://www.us-cert.gov/sites/default/files/recommended_practices/NCCIC_ICS-CERT_Defense_in_Depth_2016_S508C.pdf)
