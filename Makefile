CC=g++
CFLAGS=-Wall
INCLUDES=-Isrc/
LIBS=-lSDL2 -lGL -lGLEW -lSOIL
SRCS=Camera.cpp GLSLShader.cpp MainGame.cpp Errors.cpp InputManager.cpp Texture.cpp FPSLimiter.cpp main.cpp VertexBuffers.cpp
OBJS=$(join $(addsuffix objs/, $(dir $(SRCS))), $(notdir $(SRCS:.cpp=.o)))
MAIN=game

all: clean $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o objs/$(MAIN) $(OBJS) $(LIBS)

objs/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) objs/$(MAIN)