# Astralis

## Overview

This is a 2D orbital simulator that allows users to create and visualize celestial objects such as stars, planets, asteroids, and comets. The simulation supports various interactions, including gravity, collisions, and trajectory visualization.

## Features

- Realistic gravity simulation
- Various celestial objects (stars, planets, asteroids, comets)
- Customizable object properties
- Collision handling
- Particle effects for visual enhancements
- Save and load simulation states using JSON
- Asynchronous trajectory calculation for better performance

## Installation

No external dependencies need to be installed manually, as all required libraries are included in the project.

### Steps:

1. Clone the repository:
   ```sh
   git clone <repository_url>
   cd <project_directory>
   ```
2. Build the project:
   ```sh
   make
   ```
3. Run the simulator:
   ```sh
   ./build/main.exe
   ```

## Controls

- **Pan:** Mouse drag
- **Editing Mode:** Ctrl
- **Add object:** Use the editing tab
- **Modify object:** Right Click on an object in editing mode and adjust its properties
- **Pause/Resume:** Spacebar
- **Left/Right Arrows:** Change Time Rate
- **Save Simulation:** Simulations are automatically save when closed
- **Load Simulation:** Choose a saved file from the list

## File Storage

Simulations are saved in JSON format and stored in the `data/` directory within the project folder.
