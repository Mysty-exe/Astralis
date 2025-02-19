CXX = g++
CXXFLAGS = -std=c++20 -I./include -I./libs -I./libs/SDL2/include -I./libs/SDL2Image/include -I./libs/SDL2TTF/include -I./libs/SDL2Mixer/include
LDFLAGS = -L./libs/SDL2/lib -L./libs/SDL2Image/lib -L./libs/SDL2TTF/lib -L./libs/SDL2Mixer/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Target executable name
TARGET = build/main.exe

# Source files
SRC_FILES = src/main.cpp \
			src/core/StateManager.cpp src/core/Menu.cpp src/core/SimulationHandler.cpp src/core/Simulation.cpp src/core/Sidebar.cpp src/core/Particles.cpp src/core/Events.cpp src/core/Timer.cpp \
            src/entities/CelestialObject.cpp \
			src/physics/Vector.cpp src/physics/Utilities.cpp \
			src/ui/Image.cpp src/ui/Frame.cpp src/ui/Animation.cpp src/ui/Transition.cpp src/ui/Button.cpp src/ui/TextInput.cpp src/ui/CheckBox.cpp

# Rule to build the target
$(TARGET): $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o $(TARGET) $(LDFLAGS)

# Run the executable
run: $(TARGET)
	$(TARGET)

# Clean up generated files
clean:
	del /f /q $(TARGET)
