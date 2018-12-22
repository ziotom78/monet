# Monet − Vector graphics library for C++11

Monet is a header-only vector graphics library written in C++11. The
aim of Monet is to provide some simple way to produce 2D graphics; it
does not aim to implement complex patterns like gradients,
transparencies, and so on.

Monet does not have any dependencies other than the standard C++
library. It has been tested using GCC under Linux, but it should
compile with any compiler supporting C++11 under any operating system,
as it does not implements a GUI: image files are directly written to
disk.

At the moment, Monet supports the production of SVG files; other
formats, like EPS and PDF, are planned.

You can proceed with the [tutorial](./tutorial.md), or you can
directly download the library and start coding:

    curl https://raw.githubusercontent.com/ziotom78/monet/master/include/monet.hpp > monet.hpp
