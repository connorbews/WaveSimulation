CC := g++
CFLAGS := -I./include
LDFLAGS := -L./lib -lGL -lGLU -lglfw3 -lfftw3 -lm
SRCDIR := src/WaveSimProj
OBJDIR := obj

SOURCES := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

TARGET := a.out

$(TARGET): main.o $(OBJECTS)
	$(CC) -o $@ main.o $(OBJECTS) $(SRCDIR)/glad.c $(LDFLAGS)

debug: main.o $(OBJECTS)
	$(CC) -g -o $@ main.o $(OBJECTS) $(SRCDIR)/glad.c $(LDFLAGS)

main.o: main.cpp
	g++ -I./include -c main.cpp

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJECTS) main.o