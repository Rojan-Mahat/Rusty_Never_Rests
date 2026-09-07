# Rusty Never Rests
A 2D endless runner game built in C++17 with SFML.
<img width="1470" height="956" alt="hero" src="https://github.com/user-attachments/assets/9803dabe-3ce5-4ca6-9914-aad7d0e8e509" />

# About
Rusty Never Rests is an endless survival game in a flooded, ruined industrial world.
Survive incoming enemies and hazards, dive beneath the surface to avoid danger, manage health and oxygen, collect temporary power-ups, and keep pushing for a higher score.

## Features
- Running, jumping, diving, swimming, and flight
- Surface and underwater gameplay
- Different enemies and fireball hazards
- Health and oxygen systems
- Temporary power-ups such as shield and flight
- Persistent best score and increasing difficulty

## Gameplay
### Cover
<img alt="cover" src="https://github.com/user-attachments/assets/ea7cfcb2-227c-4466-b2e6-08307f3469d0" />

### Shield
<img src="https://github.com/user-attachments/assets/a3e2d636-94ec-4719-9f0b-e62290b0d362" alt="Rusty Never Rests gameplay"/>

### Flight
<img src="https://github.com/user-attachments/assets/24beb270-0738-4b4c-86a5-5859914acfe5" alt="Rusty Never Rests gameplay"/>

### Swimming
<img alt="swimming" src="https://github.com/user-attachments/assets/1ee9da0f-9534-4bd5-8bad-a10c83e58e57" />



## Controls
| Control | Action |
| --- | --- |
| `Space` / `W` / `Up Arrow` | Jump / Surface |
| `S` / `Down Arrow` | Dive |
| `Enter` | Emergency Shield |
| `P` | Pause |

## Built With
- C++17
- SFML 3
- CMake
- Git
- Blender

## Build and Run

### Requirements
- C++17 compatible compiler
- CMake 3.20+
- SFML 3

### Build

```bash
cmake -S . -B build
cmake --build build
```

### Run

```bash
./build/RustyNeverRests
```

## Development
Rusty Never Rests started as a small runner project inspired by the Chrome Dino game. As I kept working on it, I started adding my own ideas and it gradually grew into a complete game.
While building it, I learned more about working with multiple C++ files, game architecture, SFML, animation, collision handling, audio, CMake, Git, and asset management.
The game was built incrementally over about a week, and the Git history shows its progress from the first working jump mechanic to the completed v1 release.

## Author
Rojan Mahat

