CC=g++
CFLAGS=-Wall -std=gnu++0x
INCLUDES=-Iinclude/
LIBS=-lSDL2 -lGL -lGLEW
SRCS=Camera.cpp GLSLShader.cpp MainGame.cpp Errors.cpp InputManager.cpp Texture.cpp FPSLimiter.cpp main.cpp VertexBuffers.cpp
OBJS=$(join $(addsuffix objs/, $(dir $(SRCS))), $(notdir $(SRCS:.cpp=.o)))
DEPOBJS=objs/libsoil2.a
MAIN=game
MAKEFILE_PATH = $(abspath $(lastword $(MAKEFILE_LIST)))

all: clean $(MAIN)

$(MAIN): $(DEPOBJS) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o objs/$(MAIN) $(OBJS) $(DEPOBJS) $(LIBS)

objs/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objs/libsoil2.a:
	cd deps/SOIL2/make/linux; \
	make config=release
	cp deps/SOIL2/src/SOIL2/SOIL2.h include/
	cp deps/SOIL2/lib/linux/libsoil2.a objs/

clean:
	rm -f $(OBJS) $(DEPOBJS) objs/$(MAIN)
