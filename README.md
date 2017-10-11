# Uplink

Welcome to Uplink, Occipital's cross-platform RGBD streaming library.

It was created to connect together desktop applications and the [Structure Sensor](http://structure.io) running on an iOS mobile device.

And of course, this is the code behind [Skanect](http://skanect.com)'s Uplink feature.

It is written in C++ 98 (with some occasional C++ 11), and is mostly header-only.

## Supported Transmit-Side Platforms

### iOS

You can stream from an iOS device with Structure Sensor plugged into it.  *NOTE 2017.10.11 -- A transmit app is not currently provided, but is planned. We will remove this message and provide instructions with how to access it when it is available.*

## Supported Receive-Side Platforms

### Windows

An example server can be found [here](./examples/example-desktop-server.cpp).

### Other Platforms

The source code provided may be made to compile with modest effort on other platforms. Please submit a pull request if you manage to get it working on other platforms.

## Getting Started

Both a CMake [project](./CMakeLists.txt) and platform-specific [projects](./projects) (Visual Studio 2012 & 2013) are maintained.

Pick your favorite, then simply build and run.

No non-standard external dependencies are required, other than the tiny ones already included for the desktop server UI.
