# Tetris Game (C++)

A simple Tetris game implemented in C++.

## Features
- Classic Tetris gameplay
- Keyboard controls (arrow keys)
- Block rotation and movement
- Game over detection
- Console-based UI

## Build Instructions
1. Make sure you have a C++ compiler (e.g., g++, MSVC).
2. Clone this repository.
3. Build using the provided source files:
   
   ```sh
   g++ -std=c++17 -o tetris.exe tetris.cpp block.cpp blockAction.cpp keyAction.cpp playAction.cpp
   ```
   
   Or use the provided VSCode tasks for Windows/MSYS2.

## Run

```
./tetris.exe
```

## Controls
- **Left/Right/Down Arrow**: Move block
- **Up Arrow**: Rotate block
- **Space**: (Reserved for future use)

## License
MIT License 