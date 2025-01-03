# MockNav - The Mockup Navigator

A tool to visually turn UI mockups into navigable HTML pages.

## Dependencies

This project uses:

* [SDL2](https://github.com/libsdl-org/SDL) for windowing & rendering.

* [Dear ImGui](https://github.com/ocornut/imgui) for the user interface.

* [STB Public Domain Libraries](https://github.com/nothings/stb), in particular `stb_image.h` to load images.

ImGui and STB are included as submodules.

## Building

First, clone the repo (make sure to use the `--recursive` flag so as to clone all submodules):

```Bash
git clone --recursive https://github.com/grnln/mocknav
```

### Windows (using MSVC)

First, download [SDL2](https://github.com/libsdl-org/SDL/releases) (make sure to select the `devel-VC` package), then:

* Extract the `include` folder in the `prj/vs2022/lib` folder (if the target folder does not exist, create it).

* Extract the contents of the `lib/x86` folder to the `prj/vs2022/lib/lib` folder (if the target folder does not exist, create it).

Visual Studio 2022 with the C/C++ Desktop Development workload is required to build the project.

For that, just open the `prj/vs2022/mocknav/mocknav.sln` file using Visual Studio and compile the project in the `Debug-x86` or `Release-x86` configuration.

x64 configurations exist, but have not been tested (take into account that you should unzip the `lib/x64` folder of the SDL2 package for a 64-bit build).

### Linux (using GCC)

First, use your distribution's package manager to install the `sdl2-devel` package. For example, on Debian:

```Bash
sudo apt-get install libsdl2-dev
```

If you use Visual Studio Code, there are premade configurations to build, run, debug and clean the project.

Otherwise:

* To build the project in debug mode, use:

```Bash
make -f ./prj/gcc/Makefile debug=true
```

* To build the project in release mode, use:

```Bash
make -f ./prj/gcc/Makefile debug=false
```

* To clean the project, use:

```Bash
make -f ./prj/gcc/Makefile clean
```

The output will be in the `bin` folder.