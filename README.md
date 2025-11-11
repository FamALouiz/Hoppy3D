# Hoppy3D - Ancient Warriors 3D Collectibles Game

A 3D collectibles game built with OpenGL and FreeGLUT featuring a warrior character navigating through platforms, collecting items, and unlocking animated ancient artifacts within a time limit.

## Game Features

### Core Gameplay

- **Player Character**: 3D warrior model with sword on back, featuring smooth movement and rotation
- **Platform System**: Four color-coded platforms (red, blue, green, yellow) with unique collectibles
- **Time Limit Challenge**: Complete all collections before time runs out
- **Multiple Camera Modes**: Switch between free camera, top view, side view, front view, and third-person perspective
- **Ancient Artifacts**: Unlock animated objects by collecting all items on each platform

### Collectibles

- **Torus Coins**: Colorful collectible items (4 per platform, 16 total)
  - Animated with rotation and bobbing effects
  - Color-coded to match their platform (red, blue, green, yellow)
  - Collision detection with visual and audio feedback

### Animated Ancient Artifacts

Each platform unlocks a unique animated artifact when all collectibles are gathered:

- **Samurai Helmet** (Rotation Animation) - Continuously rotates around Y-axis
- **Ninja Star** (Scaling Animation) - Pulses in size
- **Pagoda Tower** (Transformation Animation) - Moves up and down
- **Dragon** (Color Change Animation) - Cycles through color variations

### 3D Graphics & Rendering

- **3D Primitives**: Cubes, spheres, cones, cylinders, and tori for object construction
- **Lighting System**: OpenGL lighting with ambient, diffuse, and specular components
- **Depth Testing**: Proper 3D depth rendering
- **Color Material**: Dynamic color changes for objects
- **Grid Floor**: Patterned ground with visual grid

### Physics & Collision

- **3D Physics Engine**: Custom physics with gravity, velocity, and 3D movement
- **Collision Detection**: Sphere-based collision for player and collectibles
- **Platform Interaction**: Jump mechanics with platform landing and collision
- **Boundary Detection**: Movement constraints within game arena

## Prerequisites

- MinGW-w64 (g++ compiler with C++11 support)
- Make (for building)
- FreeGLUT (included in `lib/` and `bin/`)
- OpenGL libraries (included with Windows)
- Windows Multimedia Library (winmm.lib for sound)

## Project Structure

```
Hoppy3D/
├── src/
│   ├── main.cpp                    # Entry point and GLUT initialization
│   └── game3d.cpp                  # Main game logic and 3D rendering
├── include/
│   ├── game3d.h                    # Game classes and structures
│   └── GL/                         # FreeGLUT headers
├── assets/
│   ├── fonts/                      # Font resources
│   └── sounds/                     # Sound effects (WAV files)
├── lib/x64/                        # FreeGLUT library files
├── bin/x64/                        # FreeGLUT DLL
├── build/                          # Compiled object files (generated)
├── bin/                            # Executable output (generated)
├── Makefile                        # Build configuration
├── LICENSE                         # License file
└── README.md                       # This file
```

## Game Architecture

### Core Classes

- **Game3D**: Main game manager (singleton pattern)
  - Manages game state (PLAYING, WON, GAMEOVER)
  - Handles input and updates
  - Coordinates all game objects
- **Player3D**: 3D warrior character
  - 3D model with body, head, arms, legs, and sword
  - Movement in X and Z directions
  - Jump mechanics with gravity
  - Rotation based on movement direction
- **Platform3D**: Colored platforms
  - Four platforms arranged in game space
  - Collision detection for player landing
  - Color-coded to match collectibles
- **Collectible3D**: Torus-shaped collectibles
  - Animated rotation and bobbing
  - Platform-specific tracking
  - Collision detection with player
- **AnimatedObject3D**: Ancient artifact models
  - Four types: Samurai Helmet, Ninja Star, Pagoda, Dragon
  - Four animation types: Rotation, Scaling, Transformation, Color Change
  - Unlocked when platform collectibles are complete
- **Camera3D**: Multi-mode camera system
  - Free camera with mouse control
  - Fixed views: Top, Side, Front
  - Third-person follow camera

## Building the Project

### Using Make (Command Line)

```bash
# Build the project
make

# Build and run
make run

# Clean build files
make clean

# Show help
make help
```

## Running the Application

After building, the executable will be in the `bin/` directory:

```bash
# Run directly
./bin/Hoppy3D.exe

# Or using make
make run
```

The game window opens at 800x600 resolution.

Note: Sound may not work on all systems.

## Controls

### Movement

- **W / Up Arrow**: Move forward (North)
- **S / Down Arrow**: Move backward (South)
- **A / Left Arrow**: Move left (West)
- **D / Right Arrow**: Move right (East)
- **Space**: Jump

### Camera Controls

- **1**: Free camera mode (mouse to look around)
- **2**: Top view camera
- **3**: Side view camera
- **4**: Front view camera
- **5**: Third-person camera (default)
- **Mouse Movement** (Free camera mode): Rotate view

### Game Controls

- **M**: Toggle music on/off
- **ESC**: Exit game

## Gameplay

### Objective

Collect all 16 torus coins across the four platforms before time runs out to unlock all ancient artifacts and win the game.

### How to Play

1. **Navigate the Arena**: Use WASD or arrow keys to move your warrior character
2. **Jump Between Platforms**: Press Space to jump and reach different platforms
3. **Collect Torus Coins**: Walk over the rotating, bobbing coins to collect them
   - Red coins on the red platform
   - Blue coins on the blue platform
   - Green coins on the green platform
   - Yellow coins on the yellow platform
4. **Unlock Artifacts**: Collect all 4 coins on a platform to unlock and animate its artifact
   - Complete red platform → Samurai Helmet appears and rotates
   - Complete blue platform → Ninja Star appears and pulses
   - Complete green platform → Pagoda appears and moves up/down
   - Complete yellow platform → Dragon appears and changes colors
5. **Win Condition**: Collect all 16 coins before time runs out
6. **Lose Condition**: Run out of time before collecting everything

### Camera Tips

- Use third-person mode (5) for normal gameplay
- Switch to top view (2) to get your bearings
- Try free camera (1) to explore the 3D models up close

## Technical Features

### 3D Rendering

- **OpenGL Legacy (2.x)**: Fixed-function pipeline
- **GLUT Primitives**: Spheres, cubes, cones, cylinders, tori
- **Lighting Model**: Single light source with ambient, diffuse, and specular components
- **Depth Buffer**: Proper 3D rendering with depth testing
- **Perspective Projection**: 45° FOV with dynamic aspect ratio

### Audio System

- **Windows Multimedia API**: Background music and sound effects
- **Sound Effects**: Coin collection sound (WAV format)
- **Background Music**: Game music with toggle control

### Physics

- **Gravity**: Constant downward acceleration
- **Velocity-based Movement**: Smooth character motion
- **Platform Collision**: Landing detection on platforms
- **Boundary Constraints**: Arena walls at -4.7 to 4.7 on X and Z axes

## Game States

1. **STATE_PLAYING**: Active gameplay

   - Player can move and collect
   - Timer counts down
   - Animations active

2. **STATE_WON**: Victory condition

   - All collectibles gathered
   - Victory music plays
   - Win screen displays

3. **STATE_GAMEOVER**: Defeat condition
   - Time expired
   - Game over music plays
   - Lose screen displays

## Credits

- **FreeGLUT**: OpenGL Utility Toolkit for window management and input
- **OpenGL**: 3D graphics rendering
- **GLU**: OpenGL Utility Library for quadrics and perspective
- **Windows Multimedia API**: Audio playback system

## Development

Built as a graphics programming project demonstrating:

- 3D modeling with primitives
- Camera systems and view transformations
- Lighting and shading
- Animation systems
- Collision detection in 3D
- Game state management
- Audio integration

## License

See LICENSE file for details.
