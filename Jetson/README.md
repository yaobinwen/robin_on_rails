# NVIDIA Jetson

## References

- [1] [Jetson FAQ](https://developer.nvidia.com/embedded/faq)
- [2] [Jetson Product Lifecycle](https://developer.nvidia.com/embedded/lifecycle)
- [3] [NVIDIA Jetson Linux Driver Package Software Features (Release 32.6.1 | August 3, 2021)](https://docs.nvidia.com/jetson/archives/l4t-archived/l4t-3261/index.html#page/Tegra%20Linux%20Driver%20Package%20Development%20Guide/introduction.html#)
## What is Jetson?

[1]:

> NVIDIA® Jetson is the world's leading platform for AI at the edge.

## What is JetPack?

[1]:

> NVIDIA JetPack SDK provides the foundation for building AI applications on Jetson. It bundles all the Jetson platform software, including TensorRT, cuDNN, CUDA Toolkit, VPI, GStreamer, and OpenCV, all built on top of **Jetson Linux (L4T)** with LTS Linux kernel.

## Jetson developer kit vs Jetson module

[1]:

> Each **Jetson developer kit** includes a non-production specification **Jetson module** attached to a **reference carrier board**. Together with JetPack SDK, it is used to develop and test software for your use case. Jetson developer kits are not intended for production use.

## Jetson module families and lifecycles

| Commercial Module | Available through |
|------------------:|:------------------|
| Jetson AGX Orin | January 2028 |
| Jetson Orin NX | January 2028 |
| Jetson Orin Nano | January 2028 |
| Jetson AGX Xavier | January 2025 |
| Jetson Xavier NX | January 2026 |
| Jetson TX2 NX | February 2026 |
| Jetson TX2 | January 2025 |
| Jetson Nano | January 2027 |

The following Jetson Developer Kits have reached End of Life:
- Jetson AGX Xavier Developer Kit
- Jetson Xavier NX Developer Kit
- Jetson TX2 Developer Kit
- Jetson TX1 Developer Kit
- Jetson Nano 2GB Developer Kit
- Jetson TK1 Developer Kit

## What is the placeholder `<top>`?

[3] - "Quick Start":

> The placeholder `<top>` is used throughout this document to denote the top-level directory of the files untarred from the Jetson Linux distribution.
