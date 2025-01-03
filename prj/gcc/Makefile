cc = g++
name = mocknav

cfiles = $(wildcard src/*.cpp) $(wildcard imgui/*.cpp) imgui/misc/cpp/imgui_stdlib.cpp imgui/backends/imgui_impl_sdl2.cpp imgui/backends/imgui_impl_sdlrenderer2.cpp
objs = $(cfiles:%.cpp=prj/gcc/obj/%.o)

inc = -I./prj/gcc/lib/include -I./imgui
lib = -L./prj/gcc/lib/lib

cflags = -m32 -Wall -std=c++11
ldflags = -lgcc -lm -lstdc++ -lSDL2main -lSDL2

ifeq ($(debug), true)
	cflags += -Og -g -D_DEBUG
else
	cflags += -O2
endif

ifeq ($(OS), Windows_NT)
	cflags += -Wl,-subsystem,windows
	ldflags += -lmingw32
else
	inc += -I/usr/include/SDL2
endif

.PHONY: clean

prj/gcc/obj/%.o: %.cpp
	mkdir -p $(@D)
	$(cc) -c $(cflags) $(inc) -c $< -o $@

all: $(objs)
	$(cc) $(cflags) $(objs) $(ldflags) $(lib) -o bin/$(name)

clean:
	rm -f bin/$(name)
	rm -rf prj/gcc/obj