# Game Engine

## Overview
This directory houses the core game engine used by both the client and server for the R-Type game. This engine is designed to be modular and reusable, focusing primarily on rendering, physics, and game mechanics.

## Features
- **Modular Architecture**: Enables flexible game development and easy integration with various components.
- **Physics and Collision Detection**: Supports complex interactions within the game environment.
- **Rendering Pipeline**: Optimized for 2D graphics to enhance performance and visual output.

## Integration
The game engine is integrated directly into both the client and server builds, providing core functionality without requiring separate compilation.

## Linking the game engine and the network
The game engine is linked with the network functionalities by using a Registry class instance anywhere necessary.
