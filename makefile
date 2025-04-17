CXX = g++
CXXFLAGS = -std=c++20 -I./include -I./libs -I./libs/SDL2/include -I./libs/SDL2Image/include -I./libs/SDL2TTF/include -I./libs/SDL2Mixer/include
LDFLAGS = -L./libs/SDL2/lib -L./libs/SDL2Image/lib -L./libs/SDL2TTF/lib -L./libs/SDL2Mixer/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

# Target executable name
TARGET = build/Astralis.exe

# Source files
SRC_FILES = src/main.cpp \
			src/Managers/Manager.cpp src/Managers/StateManager.cpp src/Managers/MenuManager.cpp src/Managers/SimulationManager.cpp src/Managers/EventsManager.cpp src/Managers/ParticleManager.cpp src/Managers/MessageManager.cpp \
			src/Tools/Camera.cpp src/Tools/Timer.cpp src/Tools/Utilities.cpp \
			src/Model/Simulation.cpp src/Model/CelestialObject.cpp \
			src/Math/Vector.cpp \
			src/UI/Texture.cpp src/UI/Animation.cpp src/UI/Frame.cpp src/UI/Overlay.cpp src/UI/Scrollbar.cpp src/UI/Form.cpp src/UI/Widget.cpp src/UI/Input.cpp src/UI/Button.cpp src/UI/Slider.cpp src/UI/Transition.cpp

# Rule to build the target
$(TARGET): $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o $(TARGET) $(LDFLAGS)

# Run the executable
run: $(TARGET)
	$(TARGET)

# Clean up generated files
clean:
	del /f /q $(TARGET)
