# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Iinclude -Iextern/GLAD -Iextern/GLM -Iextern/STB

# Source files
SOURCES = src/main.cpp src/graphics/shader.cpp src/graphics/texture.cpp src/camera.cpp extern/GLAD/glad.c extern/STB/stb/stb_image.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)  # Also handle .c files

# Executable name
EXECUTABLE = myapp

# Default target
all: $(EXECUTABLE)

# Rule for linking
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) -ldl -lglfw

# Rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CXXFLAGS) -c -o $@ $<

# Clean target
clean:
	rm -f src/*.o src/graphics/*.o extern/GLAD/*.o $(EXECUTABLE)

# Phony targets
.PHONY: all clean

