CC=g++
CFLAGS=-Wall -std=gnu++0x
INCLUDES=-Iinclude/
LIBS=-lSDL2 -lGL -lGLEW -lassimp
SRCS=Camera.cpp GLSLShader.cpp ShaderSystem.cpp SkyboxRenderer.cpp Skybox.cpp MainGame.cpp Errors.cpp InputManager.cpp Texture.cpp FPSLimiter.cpp main.cpp AssimpModel.cpp AssimpMesh.cpp Lighting.cpp
OBJS=$(join $(addsuffix objs/, $(dir $(SRCS))), $(notdir $(SRCS:.cpp=.o)))
DEPOBJS=objs/libsoil2.a
MAIN=ls3d

all: clean deps engine

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o objs/$(MAIN) $(OBJS) $(DEPOBJS) $(LIBS)

engine: $(MAIN)

deps: $(DEPOBJS)

objs/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

objs/libsoil2.a:
	cd deps/SOIL2/make/linux; \
	make config=release
	cp deps/SOIL2/src/SOIL2/SOIL2.h include/
	cp deps/SOIL2/lib/linux/libsoil2.a objs/

clean:
	rm -f $(OBJS) $(DEPOBJS) objs/$(MAIN)
