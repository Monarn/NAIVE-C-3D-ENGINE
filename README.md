# NAIVE-C-3D-ENGINE

BASICALLY A 3D ENGINE WRITTEN IN C WITH RAYLIB (ONLY USED 2D LINES FOR THAT)

Written for Linux, probably not cross-platform 

Written for educational purposes only, do not use it.

# Dependencies

- raylib
- x11
- glfw

# Build

It is built with a `build.sh` script. To build, run :

```shell
chmod 744 build.sh
./build.sh

./demo # run it
```


# Usage

`demo.c` is a simple example of how to use this engine. It consists of 9000 cubes whose positions are calculated thanks to a mathematic formula. Basically, it just provides `void init(Renderer*)` which is called on init, and `void run(Renderer*)` which is a function called on each frame.

Displacement keys are z q s d, and shift to go faster.

# Demo

![gif][./public/3d-engine.gif]
