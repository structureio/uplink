# Uplink

Welcome to Uplink, Occipital's cross-platform RGBD streaming library.

It was created to connect together desktop applications and the [Structure Sensor](http://structure.io) running on an iOS mobile device.

And of course, this is the code behind [Skanect](http://skanect.com)'s Uplink feature.

It is written in C++ 98 (with some occasional C++ 11), and is mostly header-only.

## Supported Platforms

### Windows

An example server can be found [here](./examples/example-desktop-server.cpp).

### Mac OS X

Complete server code is not included, yet.

### iOS

Complete client code is not included, yet.

## Getting Started

Both a CMake [project](./CMakeLists.txt) and platform-specific [projects](./projects) (Visual Studio 2012 & 2013) are maintained.

Pick your favorite, then simply build and run.

No non-standard external dependencies are required, other than the tiny ones already included for the desktop server UI.
