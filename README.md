# Hoppy - 2D Platformer Game

A 2D platformer game built with OpenGL 2.1 and FreeGLUT featuring a knight character navigating through platforms, avoiding meteors and rising lava, collecting power-ups, and reaching the final gate to win.

## Game Features

### Core Gameplay

- **Player Character**: Animated knight sprite with idle, running, jumping, and hit animations
- **Platform Generation**: Procedurally generated platforms with random sizes and textures
- **Rising Lava**: Dynamic lava that rises over time, increasing difficulty
- **Meteor System**: Falling meteors with rotation, collision detection, and warning indicators
- **Gate System**: Final door that opens with an animation when the player reaches it with the super key

### Power-ups & Collectibles

- **Keys**: Collectible coins that track progress
- **Super Key**: Special key that spawns the winning gate
- **Shield**: Grants temporary invincibility
- **Lava Freeze**: Temporarily stops lava from rising

### Visual Features

- **Sprite-based Graphics**: Knight character, power-ups, and HUD elements
- **Texture Support**: PNG textures loaded via stb_image
- **Animations**: Smooth character animations, bobbing power-ups, rotating meteors, opening door
- **Screen Backgrounds**: Custom backgrounds for start, win, and lose screens
- **Visual Feedback**:
  - Player invincibility flashing
  - Red border warning system based on lava proximity (critical/warning/safe levels)
  - Meteor warning indicators at screen top
  - HUD with hearts, power-up timers, and key counter

### Physics & Collision

- **Physics Engine**: Custom physics system with gravity, velocity, and acceleration
- **Collision Detection**: Rectangle and circle collision for different object types
- **Platform Interaction**: Jump mechanics and ground detection

## Prerequisites

- MinGW-w64 (g++ compiler with C++11 support)
- Make (for building)
- FreeGLUT (included in `lib/` and `bin/`)
- OpenGL libraries (included with Windows)
- stb_image header (included in `include/`)

## Project Structure

```
Hoppy/
├── src/
│   ├── main.cpp                    # Entry point
│   ├── player.cpp                  # Player character logic
│   ├── platform.cpp                # Platform objects
│   ├── gate.cpp                    # Winning gate with door animation
│   ├── meteor.cpp                  # Meteor objects with rotation
│   ├── lava.cpp                    # Rising lava system
│   ├── background.cpp              # Scrolling background
│   ├── hud.cpp                     # Heads-up display
│   ├── utils.cpp                   # Utility functions
│   ├── game_window/
│   │   ├── game_window.cpp         # Window management
│   │   ├── game_screen.cpp         # Base screen class
│   │   └── utils.cpp               # Window utilities
│   ├── physics/
│   │   └── objects.cpp             # Physics objects
│   ├── generators/
│   │   ├── platform_generator.cpp  # Platform generation
│   │   ├── meteor_generator.cpp    # Meteor spawning
│   │   ├── powerup_generator.cpp   # Power-up spawning
│   │   ├── powerup_manager.cpp     # Active power-up management
│   │   └── powerups/
│   │       ├── key.cpp             # Collectible keys
│   │       ├── super_key.cpp       # Special winning key
│   │       ├── shield.cpp          # Shield power-up
│   │       └── lava_freeze.cpp     # Lava freeze power-up
│   └── screens/
│       ├── start_screen.cpp        # Main menu screen
│       ├── main_screen.cpp         # Game screen
│       ├── win_screen.cpp          # Victory screen
│       └── end_screen.cpp          # Game over screen
├── include/                        # Header files
├── assets/
│   ├── sprites/                    # PNG sprite sheets and textures
│   ├── fonts/                      # Font resources
│   ├── music/                      # Background music
│   └── sounds/                     # Sound effects
├── lib/x64/                        # FreeGLUT library files
├── bin/x64/                        # FreeGLUT DLL
├── build/                          # Compiled object files (generated)
├── bin/                            # Executable output (generated)
├── Makefile                        # Build configuration
└── .vscode/                        # VS Code configuration
```

## Building the Project

### Using Make (Command Line)

```bash
# Build the project
make

# Build and run
make run

# Clean build files
make clean

# Clean everything including binaries
make distclean

# Show help
make help
```

### Using VS Code

1. **Build**: Press `Ctrl+Shift+B` or use Terminal → Run Build Task
2. **Build and Run**: Press `Ctrl+Shift+T` (Run Test Task) and select "Build and Run"
3. **Clean**: Run task "Clean Build" from the command palette

The project uses automated builds with VS Code tasks configured in `.vscode/tasks.json`.

## Running the Application

After building, the executable will be in the `bin/` directory:

```bash
# Run directly
./bin/Hoppy.exe

# Or using make
make run
```

The game starts in fullscreen mode by default (800x600 resolution).

## Controls

- **A / Left Arrow**: Move left
- **D / Right Arrow**: Move right
- **W / Up Arrow / Space**: Jump
- **ESC**: Exit game / Quit
- **Space**: Start game from menu

## Gameplay

1. Navigate platforms by moving left/right and jumping
2. Avoid falling meteors (watch for warning indicators at the top)
3. Stay above the rising lava
4. Collect keys and power-ups
5. Find the super key to spawn the final gate
6. Reach the gate to win

## Credits

- FreeGLUT for OpenGL windowing
- stb_image for PNG texture loading
- OpenGL 2.1 for graphics rendering

## License

See LICENSE file for details.
