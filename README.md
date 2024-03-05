# Bandwidth
  a memory bandwidth benchmark

[zsmith.co](https://zsmith.co/bandwidth.php)

This program is open source and covered by the GPL license. Although I wrote it mainly for my own benefit, I am also providing it pro bono, i.e. for the public good.

The latest bandwidth supports:

*  32- and 64-bit x86 running GNU/Linux
*  64-bit RISC-V running Linux
*  32- and 64-bit ARM running Linux
*  64-bit ARM computers running MacOS
*  64-bit x86 running MacOS
*  64-bit x86 running Windows

## Quick Start with Terminal

```shell
  cd ./bandwidth-1.14.10
```

```shell
  ./configure
```

```shell
  make bandwidth64
```

```shell
  ./bandwidth64 --h
```

```shell
  ./bandwidth64 --fastest
```

## Apple Silicon

  Apple's unified memory architecture puts the DRAM closer to the CPU, allowing for high bandwidth and low latency. In the case of the M1, sequential reads from main memory hover around 50 GB/second and writes are around 30 GB/second.

## License

![Static Badge](https://img.shields.io/badge/GPL-Malt?style=for-the-badge&label=License)

