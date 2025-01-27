# R-Type Server 

## Table of Contents

1. [Overview](#1-overview)
2. [Prerequisites](#2-prerequisites)
3. [Building](#3-building)
4. [Architecture](#4-architecture)
5. [Adding New Features](#5-adding-new-features)
6. [Configuration](#6-configuration)
7. [Contributing](#7-contributing)

---

## 1. Overview

This server implements a multiplayer game engine with:

- Entity Component System (ECS)
- Room-based multiplayer
- Multiple difficulty levels
- Configurable game mechanics
- TCP/UDP protocol support

---

## 2. Prerequisites

- CMake (3.17+)
- C++17 compatible compiler
- nlohmann/json library
- Networking libraries (Asio)

---

## 3. Building

```bash
mkdir build
cd build
cmake ..
make
```

---

## 4. Architecture

The server is built on several key systems:

1. **Entity Component System**
- Components for health, position, velocity, etc.
- Systems for movement, collision, shooting
- Factory pattern for entity creation
2. **Game Engine**
- Manages game loop and state
- Handles entity spawning and updates
- Processes player input
3. **Room Management**
- Multiplayer session handling
- Player state synchronization
- Game state management
4. **Protocol System**
- TCP for reliable communication
- UDP for real-time updates
- Custom command handling

---

## 5. Adding New Features

### Adding New Components

1. Create a new component class:
```cpp
class NewComponent : public Component {
public:
    // Add component properties
    int someValue;
    
    NewComponent(int value) : someValue(value) {}
};
```
2. Register it in the entity factory system where needed:
```cpp
entity->addComponent(std::make_shared<NewComponent>(value));
```

### Adding New Systems

1. Create a new system class:
```cpp
class NewSystem : public System {
public:
    void update(std::shared_ptr<Room> room, Entity& entity, float deltaTime) override {
        auto component = entity.getComponent<NewComponent>();
        if (!component) return;
        
        // Implement system logic
    }
};
```
2. Add it to the GameEngine update loop:
```cpp
// In GameEngine::update
newSystem.update(room, *entity, deltaTime);
```

### Adding New Protocol Commands

1. Define your command handler:
```cpp
void handleNewCommand(const std::string& data, std::shared_ptr<Player> player) {
    // Implement command logic
}
```
2. Register it in the protocol handlers:
```cpp
tcp_command_handlers = {
    {"NEW_CMD", std::bind(&Protocol::handleNewCommand, this, std::placeholders::_1, std::placeholders::_2)},
    // ... other handlers
};
```

### Adding New Entity Types

1. Add configuration in the appropriate JSON files:
```cpp
{
    "NEW_ENTITY": {
        "health": 100,
        "speed": 2.0,
        "reward": 10,
        "mouvement": {
            "pattern": "LINEAR",
            "direction": "LEFT"
        },
        "hitbox": {
            "width": 32,
            "height": 32
        },
        "bulletType": "BASIC",
        "timeRemaining": 0
    }
}
```
2. Create a factory method if needed:
```cpp
static std::shared_ptr<Entity> createNewEntity(/* parameters */) {
    auto entity = std::make_shared<Entity>(id, group, type);
    // Add components
    return entity;
}
```

### Adding New Game Modes

1. Create configuration files for the new mode
2. Implement mode-specific logic in a new system
3. Add mode selection in the room management system

---

## 6. Configuration

The server uses JSON configuration files for:

- Level design (`levels_*.json`)
- Entity properties (`entitys_*.json`)
- Bullet properties (`bullets_*.json`)

Configuration files are organized by difficulty level:
```cpp
server/Config/
├── Easy/
│   ├── levels_easy.json
│   ├── entitys_easy.json
│   └── bullets_easy.json
├── Medium/
│   └── ...
└── Hard/
    └── ...
```

---

## 7. Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Write/update tests if necessary
5. Submit a pull request

### Code Style Guidelines

- Use smart pointers for memory management
- Follow existing naming conventions
- Document new features and systems
- Include error handling
- Write clean, maintainable code

For more detailed information, refer to the Server Implementation Guide.

---

**[Epitech] Technical Documentation © 2024**
