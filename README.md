# SpriteSparkEngine

**SpriteSparkEngine** is a 2D game engine developed in C++ with a focus on modern, efficient game development practices. This project was created as part of a Bachelor's thesis at the SRH Hochschule Heidelberg, showcasing a comprehensive implementation of a data-driven architecture and leveraging the Vulkan API for high-performance graphics rendering.

## Project Overview

The primary objective of SpriteSparkEngine is to provide a flexible and powerful foundation for 2D game development, enabling developers to create high-quality games with efficient resource management and scalable architecture. The engine's data-driven approach separates data from logic, making the engine highly adaptable to different game genres and facilitating easier maintenance and expansion.

## Key Features

- **Data-Driven Architecture**: Facilitates the separation of game logic from data, making the engine highly adaptable and maintainable.
- **Vulkan API Integration**: Utilizes Vulkan for cutting-edge graphics rendering, ensuring optimal performance and cross-platform compatibility.
- **Cross-Platform Support** (Work in Progress): While designed to run on various operating systems, cross-platform support is still under development.

## Academic Context

This engine was developed as part of a Bachelor's thesis at the SRH Hochschule Heidelberg, underlining the academic rigor and practical application of advanced programming and software engineering principles.

## Development and Configuration

Developers can configure and build the project using the provided setup scripts for Linux, macOS, and Windows or by manually running the Premake command with their preferred IDE. The engine is modular and extendable, allowing for easy integration of additional libraries or features.


## Getting Started

### Prerequisites

Ensure you have the following installed:

- **C++17** or higher
- **Vulkan SDK** from https://www.lunarg.com/vulkan-sdk/
- **OpenAL SDK** from https://www.openal.org/downloads/
- **CMake** or **Premake** for build configuration

### Building the Engine

Follow these steps to build the engine:

1. Clone the repository:
   ```bash
   git clone https://github.com/WiSie98/SpriteSparkEngine.git

2. Navigate to the project directory:
   ```bash
   cd SpriteSparkEngine\Scripts

3. Generate project files:
   
   You can configure the Premake project by either using the provided batch files (Setup-Linux.sh, Setup-MacOS.sh, Setup-Windows.bat) or by manually running the following command:
   ```bash
   premake5 gmake  # for makefiles
   premake5 vs2022 # for a Visual Studio 2022 solution
   premake --help  # to see a list of supported toolsets

## External Libraries

The following external libraries are utilized in the SpriteSparkEngine to provide essential functionality:

- **GLFW**: 
  - Used for handling input and managing the window context. 
  - [GLFW](https://www.glfw.org) is a lightweight library for creating windows with OpenGL contexts and handling user input.

- **GLM**: 
  - Employed for SIMD-accelerated vector operations and other mathematical functions related to rendering.
  - [GLM](https://github.com/g-truc/glm) is a header-only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.

- **OpenAL**: 
  - Used for implementing all sound and music functionalities within the engine.
  - [OpenAL](https://www.openal.org) is a cross-platform audio API designed for efficient rendering of multichannel three-dimensional positional audio.

- **Vulkan**: 
  - Utilized for all rendering processes within the engine.
  - [Vulkan](https://www.khronos.org/vulkan/) is a modern cross-platform graphics and compute API that provides high-efficiency, cross-platform access to modern GPUs.

- **dr_flac.h**, **dr_mp3.h**, **dr_wav.h**: 
  - These headers are used to import sound files in FLAC, MP3, and WAV formats, respectively.
  - They are part of the [dr_libs](https://github.com/mackron/dr_libs/tree/master) single-file public domain libraries for audio decoding.

- **json.h**: 
  - This header is used to import and convert Tiled tilesets and tilemaps into the engine’s format.
  - It is a minimalistic [JSON parser](https://github.com/nlohmann/json) written in C, used for lightweight and easy-to-integrate JSON parsing.

- **stb_image.h**: 
  - Used to import images in various formats for use within the engine.
  - [stb_image](https://github.com/nothings/stb) is a popular, public domain, single-file library for image loading.

# Project Structure and Usage

## Directory Structure

The `SpriteSparkGame` directory contains the following important subdirectories and files:

- **Levels/**:
  - Contains JSON files that describe the levels of the game, including tilesets and tilemaps.
  - Example: `vp_ts_metroidlevel.json`, `vp_lv_metroidlevel.json`

- **Shaders/**:
  - Stores shader files used for rendering in the engine.
  - Example: Vertex and fragment shader files specific to Vulkan rendering.

- **Sound/**:
  - Contains audio files that are used within the game.
  - Example: `Theme of Samus Aran, Space Warrior - Super Smash Bros. Ultimate.mp3`

- **Source/**:
  - Contains the source code for the game logic.
  - Example: `App.cpp` implements the main game application.

- **Textures/**:
  - Stores texture files used for rendering sprites and tilemaps in the game.
  - Example: `vp_ts_metroidlevel.png`, `vp_sptsht_player.png`

## Example Usage of the Engine (`App.cpp`)

The `App.cpp` file demonstrates how to use the SpriteSparkEngine to create a basic game. Below are the key components:

### Creating the Application
Finally, the TestApp class creates an instance of the application and adds the TestLayer to it:

```cpp
class TestApp : public SpriteSpark::Application {
public:
    TestApp() {
        PushLayer(new TestLayer());
    }
};

SpriteSpark::Application* SpriteSpark::CreateApplication() {
    return new TestApp();
}
```

### Creating a Layer

A `Layer` in SpriteSparkEngine represents a collection of related game entities and systems. In `App.cpp`, `TestLayer` is defined as follows:

```cpp
class TestLayer : public Layer {

public:
    TestLayer() : Layer("Test") {}

    void OnInit(Camera& camera) override {
        // Initialization code here
    }

    void OnUpdate(float deltaTime, Camera& camera) override {
        // Update code here
    }

    void OnRender(FrameInfo& frameInfo) override {
        // Render code here
    }
};
```

### Initializing the Layer
The OnInit function is used to initialize various components of the game, such as loading textures, sounds, and setting up entities:

```cpp
void OnInit(Camera& camera) override {
    sound.setFilepath("Sound/Theme of Samus Aran, Space Warrior - Super Smash Bros. Ultimate.mp3");
    GlobalLoader::LoadSprites(entityManager, 3, tilemap, "Levels/vp_ts_metroidlevel.json", "Levels/vp_lv_metroidlevel.json");

    // Example of creating and setting up an entity
    Entity player = entityManager.createEntity();
    entityManager.addComponent(player, Player{});
    entityManager.addComponent(player, Transform{ {240.0f, 3194.0f}, {1.0f, 1.0f}, 0.0f });
    entityManager.addComponent(player, Sprite{ playerSprite, {0.0f, 0.0f, 16.0f, 32.0f} });

    sound.play(true);
}
```

### Updating and Rendering
The OnUpdate function handles game logic such as input processing and physics simulation:

```cpp
void OnUpdate(float deltaTime, Camera& camera) override {
    if (Input::IsKeyPressed(Key::Up)) {
        volume += 0.01f;
        sound.setVolume(volume);
    }

    movementSystem.update(entityManager, camera, deltaTime);
    physicsSystem.update(entityManager, deltaTime);
    collisionSystem.update(entityManager, deltaTime);
}
```

The OnRender function is responsible for rendering the entities:

```cpp
void OnRender(FrameInfo& frameInfo) override {
    spriteSystem.render(frameInfo, entityManager);
}
```

For a more detailed overview of the engine's functionalities, please refer to the [Wiki](https://github.com/WiSie98/SpriteSparkEngine/wiki) of this repository.
