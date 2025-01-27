# R-Type Client Documentation

## Introduction

This document provides a detailed description of the client side of the R-Type game. The client is developed in C++ using the SFML graphics library. It handles graphical display, user interactions, entity management, and network communication with the server.

## Project Structure

The structure of the client project is as follows:
```
client/
    font/
    include/
        ECS/
            BossFactory.hpp
            Component.hpp
            EnnemyFactory.hpp
            Entity.hpp
            ObstacleFactory.hpp
            ParallaxBackground.hpp
            SoundManager.hpp
            System.hpp
            TextureManager.cpp
            TextureManager.hpp
            World.hpp
        Menu/
            Menu.hpp
            MenuState.hpp
        Network/
            NetworkManager.hpp
        Parser/
            jsonParser.hpp
        EntityFactory.hpp
        InputHandler.hpp

    sound/
    sprites/
    src/
        EntityFactory.cpp
        Events.cpp
        InputHandler.cpp
        Menu.cpp
        NetworkManager.cpp
        SoundManager.cpp

    main.cpp
    main.hpp
```

## Dependencies

The client uses the following libraries:
- **SFML**: For managing graphics, sound, and events.
- **Boost**: For managing threads and network operations.

## Compilation and Execution

### Prerequisites

Make sure you have the following tools and libraries installed on your system:
- **CMake**: Project configuration tool.
- **Make**: Build tool.
- **g++** : C++ compiler.
- **SFML** : Graphics library.

### Compilation

1. Clone the project repository:
    ```bash
    git clone <REPOSITORY_URL>
    cd <REPOSITORY_NAME>
    ```
2. Create a build directory:
    ```bash
    mkdir build
    cd build
    ```
3. Run CMake to configure the project:
    ```bash
    cmake ..
    ```
4. Compile the project:
    ```bash
    make
    ```

### Execution

After compilation, you can run the generated executable:
    ```bash
    ./r-type_client <ip_address> <port>
    ```

## Component Descriptions

### Main

The main.cpp file initializes the SFML window, configures the ECS systems, and starts the main game loop.

### Menu

The `Menu` class is responsible for managing the user interface and interactions within the game. It provides support for rendering various game menus, handling events, and facilitating transitions between different menu states such as the main menu, room selection, and in-game settings.

#### Key Features:

- **Menu States**: Manages transitions between different menu states using the MenuState enumeration. Supports main menu, pause menu, room selection, game over, and settings menus.
- **Room Management**: Provides functionality for room selection, creation, and lobby interactions, including password-protected rooms.
- **Custom UI Elements**: Features buttons, text inputs, sliders, and a custom cursor to enhance the user experience.
- **Accessibility**: Includes options for high contrast mode, Daltonian mode, and adjustable text scaling.
- **Dynamic Elements**: Offers real-time updates for room lists, player lobbies, and loading animations.
- **Settings Management**: Allows for customization of key bindings, volume settings, and difficulty levels.
- **Scoreboard**: Displays a scoreboard with dynamically updated scores.

#### MenuState Enum

The `MenuState` enum defines various states for the game's menus. It is used to manage the current state of the menu system and allow transitions between different states.
```cpp
// MenuState.hpp

#pragma once

enum class MenuState {
    MAIN_MENU,
    ROOM_SELECT,
    ROOM_CREATE,
    ROOM_LOBBY,
    GAME,
    PAUSE,
    GAME_OVER
};
```

#### Class Structure:

1. **Member Variables**:
- `sf::RenderWindow& window`: Reference to the game window for rendering.
- `MenuState currentState, previousState`: Tracks the current and previous menu states.
- UI Elements: Includes buttons, text fields, labels, and sliders for various menus.
- `NetworkManager networkManager`: Manages networking functionality for room data and lobby updates.
- `KeyBindings keyBindings`: Stores user-configured key mappings for gameplay.
2. **Menu-Specific Features**:
- **Room Management**:
  - `std::vector<Room> rooms`: Stores available rooms.
  - Functions for updating and rendering room lists and lobbies.
- **Accessibility**:
  - High contrast and Daltonian modes for enhanced visibility.
- **Settings Menu**:
  - Key remapping and volume control with real-time feedback.
- **Custom UI Updates**:
  - Methods like `centerTextInButton`, `updateRoomListPositions`, and `updateCursor` for maintaining consistent UI placement and interactivity.
3. **Public Methods**:
- Constructor: Initializes the menu with the game window, world reference, and network configuration.
- `void handleEvent(const sf::Event& event)`: Processes user input events.
- `void render()`: Renders the appropriate menu based on the current state.
- `void setState(MenuState state)`: Transitions to a new menu state.
- Utility Methods:
  - `initializeRooms()`, `createButtons()`, `updateRooms(const std::string& roomData)`, etc.
4. **Static Features**:
- `static StateChangeCallback globalStateChangeCallback`: Allows global state changes to be triggered from external sources.

**Usage**: The `Menu` class integrates seamlessly into the game's main loop. Initialize it with a reference to the `sf::RenderWindow` and the game world, then call its `handleEvent` and `render` methods to manage user interaction and display the appropriate menu.

Example Initialization:
```cpp
Menu menu(window, &gameWorld, "127.0.0.1", "5000");
menu.setState(MenuState::MAIN_MENU);
while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        menu.handleEvent(event);
    }
    menu.render();
    window.display();
}
```

### Event Handling

This component is responsible for managing all game events and user interactions across different menu states and gameplay scenarios. It's a crucial part of the Menu class that handles various types of events including mouse interactions, keyboard input, network messages, and game state changes.

#### Key Features:

1. **Menu State Event Handlers**:

- `handleMainMenuEvents`: Manages main menu interactions including accessibility toggles
- `handleRoomSelectEvents`: Handles room selection, creation, and joining
- `handleRoomLobbyEvents`: Processes lobby-specific events and player synchronization
- `handleRoomCreateEvents`: Manages room creation form inputs and submission
- `handleGameOverEvents`: Processes end-game state and restart options
- `handlePauseMenuEvents`: Handles pause menu interactions and game resumption
- `handlePasswordEvents`: Manages password input for protected rooms
2. **Game Event Management**:
- Real-time player movement processing
- Shooting mechanics and cooldown management
- Entity creation and destruction
- Network message processing for game state synchronization
3. **Network Integration**:
- Processes TCP/UDP messages for game state updates
- Handles player position synchronization
- Manages room state changes and player connections
4. **Input Processing**:
- Keyboard controls for player movement (ZQSD/Arrow keys)
- Mouse interaction with UI elements
- Text input handling for forms and password entry
5. **Accessibility Features**:
- High contrast mode toggle
- Font scaling and style adaptation
- Color scheme management for enhanced visibility
6. **Game State Synchronization**:
- Entity position updates
- Player action broadcasting
- Score and game state management

#### Implementation Details:

1. **Event Flow**:
```cpp
void Menu::handleGameEvents() {
    // Process network messages
    while (networkManager.hasGameStateMessages()) {
        // Handle entity creation, updates, and deletion
    }
    
    // Process player inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        // Handle shooting mechanics
    }
    
    // Handle movement inputs
    if (sf::Keyboard::isKeyPressed(keyBindings.moveUp)) {
        // Process upward movement
    }
    // ... similar handlers for other directions
}
```
2. **Network Message Processing**:
```cpp
void Menu::handleRoomLobbyEvents(const sf::Event& event) {
    while (networkManager.hasMessages()) {
        std::string message = networkManager.getNextMessage();
        std::string protocol_id = message.substr(1, 3);
        // Process different types of network messages
    }
}
```
3. **UI Event Handling**:
```cpp
void Menu::handleMainMenuEvents(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        // Handle hover effects
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        // Process button clicks
    }
}
```

This component is essential for maintaining responsive user interactions and smooth gameplay experience while ensuring proper synchronization between client and server states.

### InputHandler

The `InputHandler` class is responsible for processing input events related to user interactions, such as key presses and releases, in the game. It is integrated with the Entity Component System (ECS) to allow the modification of entities' components based on user input, particularly handling player movement and actions.

#### Key Features:

- **Input Handling**: Handles key press and release events and modifies the components of relevant entities, such as the player's position and velocity.
- **Component Modification**: Works with player-related components, such as `PlayerComponent`, `VelocityComponent`, and `PositionComponent`, to reflect input changes in the game world.
- **Sound Integration**: Triggers appropriate sound effects for player actions through the SoundManager.

#### Class Structure:

1. **Public Methods**:
- `static void handleInput(World& world, sf::Event event)`: General input handler method that handles various types of input events.
- `static void handleKeyReleased(World& world, sf::Event event)`: Specific handler for key release events.
2. **Private Methods**:
- `static void handleKeyPressed(sf::Event event, std::shared_ptr<PlayerComponent> player, std::shared_ptr<VelocityComponent> vel, std::shared_ptr<PositionComponent> pos, World& world)`: Handles key press events and updates the respective player components.

#### Implementation Details:

1. **Key Press Handling**:
```cpp
void InputHandler::handleKeyPressed(sf::Event event, std::shared_ptr<PlayerComponent> player,
    std::shared_ptr<VelocityComponent> vel,
    std::shared_ptr<PositionComponent> pos,
    World& world) {
    switch (event.key.code) {
        case sf::Keyboard::Z:
            vel->dy = -0.013f;
            player->state = PlayerComponent::AnimationState::UP;
            SoundManager::playSound("client/sound/space-ship-102433.ogg");
            break;
        case sf::Keyboard::S:
            vel->dy = 0.013f;
            player->state = PlayerComponent::AnimationState::DOWN;
            SoundManager::playSound("client/sound/space-ship-102433.ogg");
            break;
        case sf::Keyboard::Q:
            if (pos->x > -0.95f) {
                vel->dx = -0.013f;
                SoundManager::playSound("client/sound/space-ship-102433.ogg");
            }
            break;
        case sf::Keyboard::D:
            if (pos->x < 0.95f) {
                vel->dx = 0.013f;
                SoundManager::playSound("client/sound/space-ship-102433.ogg");
            }
            break;
        case sf::Keyboard::Space:
            EntityFactory::createBullet(world, pos->x, pos->y);
            SoundManager::playSound("client/sound/blaster-103340.ogg");
            break;
    }
}
```
2. **Key Release Handling**:
```cpp
void InputHandler::handleKeyReleased(World& world, sf::Event event) {
    auto entities = world.getEntities();
    for (auto& entity : entities) {
        if (entity->hasComponent<PlayerComponent>()) {
            auto player = entity->getComponent<PlayerComponent>();
            auto vel = entity->getComponent<VelocityComponent>();
            
            switch (event.key.code) {
                case sf::Keyboard::Z:
                case sf::Keyboard::S:
                    vel->dy = 0.0f;
                    player->state = PlayerComponent::AnimationState::IDLE;
                    break;
                case sf::Keyboard::Q:
                case sf::Keyboard::D:
                    vel->dx = 0.0f;
                    break;
            }
        }
    }
}
```

#### Control Scheme:

**Z**: Move upward (with sound effect)
**S**: Move downward (with sound effect)
**Q**: Move left (with boundary check at x > -0.95f)
**D**: Move right (with boundary check at x < 0.95f)
**Space**: Fire bullet (with shooting sound effect)

#### Key Features:

1. **Movement Speed**: Controlled using velocity values of ±0.013f
2. **Boundary Checking**: Prevents player from moving outside game boundaries
3. **Animation States**: Updates player animation state based on movement
4. **Sound Integration**:
- Movement sounds: "space-ship-102433.ogg"
- Shooting sounds: "blaster-103340.ogg"
5. **State Reset**: Returns to IDLE state when movement keys are released

### NetworkManager

The `NetworkManager` class handles the client-server communication, including both TCP and UDP connections. It is responsible for sending and receiving messages, managing connections, and handling game state synchronization between the client and server.

#### Key Features:

- **TCP and UDP Communication**: Manages both TCP and UDP connections to the server.
- **Connection Management**: Connects, disconnects, and checks connection status.
- **Message Queues**: Handles message queuing for TCP and UDP messages, ensuring reliable message processing.
- **Room Management**: Includes functions for requesting rooms, joining rooms, leaving rooms, and starting or ending games.
- **Game State**: Receives and processes game state messages and actions.
- **Threaded Reception**: Uses a separate thread to handle incoming messages asynchronously.
- **UDP Handling**: Specifically manages UDP message reception and sending.
- **Binary Serialization**: Uses Boost.Archive for binary serialization of messages.

#### Class Structure:
```cpp
// NetworkManager.hpp
#pragma once
#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class NetworkManager {
    public:
        NetworkManager(const std::string& host, const std::string& port);
        ~NetworkManager();
        void connect();
        void connectUDP(int player_id);
        void disconnect();
        bool isConnected() const { return connected; }
        void sendUDP(const std::string& message);
        void sendTCP(const std::string& message);
        void startReceiving();
        std::string getNextMessage();
        bool hasMessages() const;
        std::string getNextUDPMessage();
        std::string getNextGameStateMessage();
        std::string getNextActionMessage();
        bool hasUDPMessages() const;
        bool hasActionMessages() const { return !action_queue.empty(); }
        bool hasGameStateMessages() const { return !game_state_queue.empty(); }
        void requestRooms();
        void requestLeaderboard();
        int getPlayerId() const { return player_id; }
        void joinRoom(const std::string& roomId, const std::string& password);
        void leaveRoom(const std::string& roomId);
        void startGame(const std::string& roomId);
        void endGame(const std::string& roomId);
        void requestPlayersInRoom();
        void startReceivingUDP();
        void handleUDPReceive(const boost::system::error_code& error, std::size_t bytes_transferred);
        bool isInRoom() const { return m_isInRoom; }
    private:
        void receiveLoop();
        void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);
        boost::asio::io_context io_context;
        std::shared_ptr<tcp::socket> tcp_socket;
        udp::socket udp_socket;
        bool connected;
        std::string host;
        std::string port;
        udp::endpoint udp_server_endpoint;
        std::thread receive_thread;
        std::queue<std::string> message_queue;
        std::queue<std::string> udp_message_queue;
        mutable std::mutex queue_mutex;
        mutable std::mutex udp_queue_mutex;
        bool should_stop;
        bool m_isInRoom = false;
        std::array<char, 1024> receive_buffer;
        std::string receive_buffer_str;
        int player_id = -1;
        struct MessageBuffer {
            std::vector<char> data;
            size_t position;
            MessageBuffer() : position(0) {}
            void append(const char* new_data, size_t length) {
                data.insert(data.end(), new_data, new_data + length);
            }
            std::string extractMessage() {
                auto newline = std::find(data.begin() + position, data.end(), '\n');
                if (newline == data.end()) return "";
                std::string message(data.begin() + position, newline);
                position = newline - data.begin() + 1;
                if (position >= data.size()) {
                    data.clear();
                    position = 0;
                }
                return message;
            }
        };
        MessageBuffer message_buffer;
        std::array<char, 8192> udp_receive_buffer;
        MessageBuffer udp_message_buffer;
        udp::endpoint udp_sender_endpoint;
        std::queue<std::string> game_state_queue;  // For protocol 115
        std::queue<std::string> action_queue;      // For protocol 107
};
```

#### Key Methods:

1. **Constructor**: Initializes the network manager with the server host and port.
```cpp
NetworkManager(const std::string& host, const std::string& port);
```
2. **connect()**: Establishes a TCP connection to the server and initializes UDP socket.
- Sets up TCP and UDP connections
- Starts receiving threads
- Sends initial connection message (protocol 100)
3. **connectUDP()**: Establishes a UDP connection for player communication.
```cpp
void connectUDP(int player_id);
```
4. **sendTCP() / sendUDP()**: Sends  messages over respective protocols.
- TCP messages are serialized using Boost.Archive
- UDP messages are sent directly to the server endpoint
5. **Message Handling**:
- `handleReceive()`: Processes TCP messages with protocol parsing
- `handleUDPReceive()`: Processes UDP messages for game state and actions
- Protocol IDs:
  - 100: Player ID assignment
  - 101: UDP connection
  - 103: Room join confirmation
  - 104: Room list
  - 105: Game start notification
  - 107: Action messages (UDP)
  - 112/119: Player list
  - 114: Game end
  - 115: Game state (UDP)
  - 125: Leaderboard
6. **Room Management**:
```cpp
void requestRooms();
void joinRoom(const std::string& roomId, const std::string& password);
void leaveRoom(const std::string& roomId);
void startGame(const std::string& roomId);
void endGame(const std::string& roomId);
```

#### Example Usage:
```cpp
NetworkManager networkManager("127.0.0.1", "5000");
networkManager.connect();
networkManager.startReceiving();
networkManager.sendTCP("Hello, Server!");
```

#### Design Principles:

- **Threaded Network Handling**: Uses asynchronous network handling to prevent blocking the main game loop.
- **Message Queuing**: Ensures thread-safe message handling with message queues and mutexes.
- **Robust Communication**: Manages both reliable TCP and faster UDP connections.
- **Error Handling**: Comprehensive error handling for network operations and message processing.
- **Binary Serialization**: Uses Boost.Archive for efficient message serialization.

### EntityFactory

The `EntityFactory` class is responsible for creating and initializing various game entities within the ECS (Entity-Component-System) architecture. It provides static methods for creating different types of entities, including players, bullets, enemies, and bosses, which are essential for game progression.

#### Key Features:

- **Player Creation**: Initializes a player entity at a specific position with a player ID and network ID.
- **Bullet Creation**: Provides methods for creating standard and specialized bullets, including single bullets with angle offsets.
- **Boss Creation**: Instantiates a boss entity to be placed within the game world.
- **Enemy Bullet Creation**: Creates enemy bullets to be used in combat scenarios.

#### Class Structure:
```cpp
// EntityFactory.hpp
#pragma once
#include "ECS/World.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Component.hpp"
#include "ECS/TextureManager.hpp"

class EntityFactory {
public:
    static void createPlayer(World& world, float x, float y, int playerId, int netId);
    static void createBullet(World& world, float x, float y);
    static void createSingleBullet(World& world, float x, float y, float angleOffset);
    static void createBoss(World& world);
    static std::shared_ptr<Entity> createEnemyBullet(World& world, float x, float y);
};
```

#### Public Methods:

- `createPlayer(World& world, float x, float y, int playerId, int netId)`: Creates a player entity at the specified position `(x, y)` with the provided player and network IDs.
- `createBullet(World& world, float x, float y)`: Creates a standard bullet entity at the given position.
- `createSingleBullet(World& world, float x, float y, float angleOffset)`: Creates a bullet with a specified angle offset.
- `createBoss(World& world)`: Creates a boss entity within the game world.
- `createEnemyBullet(World& world, float x, float y)`: Creates an enemy bullet entity at the specified position and returns it as a shared pointer.

#### Example Usage:
```cpp
World gameWorld;
EntityFactory::createPlayer(gameWorld, 100.0f, 200.0f, 1, 12345);
EntityFactory::createBullet(gameWorld, 150.0f, 250.0f);
EntityFactory::createSingleBullet(gameWorld, 200.0f, 300.0f, 45.0f);
EntityFactory::createBoss(gameWorld);
auto enemyBullet = EntityFactory::createEnemyBullet(gameWorld, 250.0f, 350.0f);
```

#### Additional Method Details
```cpp
// client/src/EntityFactory.hpp
/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** EntityFactory
*/

#include "../include/EntityFactory.hpp"


void EntityFactory::createPlayer(World& world, float x, float y, int playerId, int netId) {
    auto player = std::make_shared<Entity>();
    
    player->addComponent<PositionComponent>(x, y);
    player->addComponent<VelocityComponent>();
    player->addComponent<PlayerComponent>();
    player->addComponent<ScoreComponent>(0);
    
    auto render = RenderComponent(RenderComponent::Type::PLAYER);
    render.texture = TextureManager::loadTexture("client/sprites/r-typesheet42.png");
    render.sprite.setTexture(*render.texture);
    render.frameRect = sf::IntRect(0, 0, 33, 18);
    render.sprite.setTextureRect(render.frameRect);
    render.sprite.setOrigin(16.5f, 18.0f);
    render.sprite.setScale(4.0f, 4.0f);
    render.sprite.setPosition(x, y);
    
    player->addComponent<RenderComponent>(render);
    player->setId(playerId);
    player->setNetId(netId);
    world.addEntity(player);
}

void EntityFactory::createBullet(World& world, float x, float y) {
    auto player = world.findEntityWithComponent<PlayerComponent>();
    if (!player) {
        createSingleBullet(world, x, y, 0.0f);
        return;
    }

    auto playerComp = player->getComponent<PlayerComponent>();
    if (!playerComp) {
        createSingleBullet(world, x, y, 0.0f);
        return;
    }
    
    if (playerComp->hasTripleShot) {
        // Central shot
        createSingleBullet(world, x, y, 0.0f);
        // Upper shot (reduced from 0.2f to 0.1f)
        createSingleBullet(world, x, y, 0.02f);
        // Lower shot (reduced from -0.2f to -0.1f)
        createSingleBullet(world, x, y, -0.02f);
    } else {
        createSingleBullet(world, x, y, 0.0f);
    }
}

void EntityFactory::createSingleBullet(World& world, float x, float y, float angleOffset) {
    auto bullet = std::make_shared<Entity>();
    bullet->addComponent<PositionComponent>(x, y);
    bullet->addComponent<BulletComponent>(0.02f, angleOffset);
    
    auto render = RenderComponent(RenderComponent::Type::BULLET);
    render.texture = TextureManager::loadTexture("client/sprites/r-type-projectile.png");
    render.sprite.setTexture(*render.texture);
    render.frameRect = sf::IntRect(0, 0, 33, 36);
    render.sprite.setTextureRect(render.frameRect);
    render.sprite.setOrigin(16.5f, 18.0f);
    render.sprite.setScale(1.0f, 1.0f);
    render.frameTime = 0.1f;
    render.frameCount = 3;
    
    float angle = (angleOffset / 0.02f) * 15.0f;
    render.sprite.setRotation(angle);
    
    bullet->addComponent<RenderComponent>(render);
    world.addEntity(bullet);
}

std::shared_ptr<Entity> EntityFactory::createEnemyBullet(World& world, float x, float y) {
    auto bullet = std::make_shared<Entity>();
    bullet->addComponent<PositionComponent>(x, y);
    bullet->addComponent<BulletComponent>(-0.02f);  // Negative speed for leftward direction

    auto render = RenderComponent(RenderComponent::Type::BULLET);
    render.texture = TextureManager::loadTexture("client/sprites/porjectile-ennemie.png");
    render.sprite.setTexture(*render.texture);
    render.frameRect = sf::IntRect(0, 0, 33, 36);
    render.sprite.setTextureRect(render.frameRect);
    render.sprite.setOrigin(16.5f, 18.0f);
    render.sprite.setScale(-1.0f, 1.0f);  // Flip the sprite
    bullet->addComponent<RenderComponent>(render);

    return bullet;
}
```

### JsonParser

The `jsonParser.hpp` file provides functionality to parse various game-related data from JSON format. It defines structures for different game entities (rooms, players, enemies, etc.) and methods to extract the relevant information from JSON data.

#### Key Features:

- **Room Parsing**: Parses room data, including room name, password, and player counts.
- **Player Parsing**: Extracts a list of player names.
- **Player and Enemy Position Parsing**: Handles the UDP positions of players and enemies, including their coordinates and health.
- **Leaderboard Parsing**: Parses leaderboard data to extract player names and scores.
- **Entity Parsing**: Extracts entity positions, including scores and health.

#### Class Structure:
```cpp
// jsonParser.hpp
#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

class JsonParser {
public:
    struct Room {
        std::string id;
        std::string name;
        std::string password;
        std::string maxPlayers;
        std::string currentPlayers;
        std::string state;
    };

    struct Player {
        std::string id;
        std::string name;
    };

    struct PlayerUDPposition {
        std::string id;
        float x;
        float y;
        std::string state;
        std::string group;
        std::string type;
        int health;
        int score;
    };

    struct EnemyUDPposition {
        std::string id;
        float x;
        float y;
        int health;
        bool isBoss;
        std::string type;
    };

    struct Leaderboard {
        std::string name;
        int score;
    };

    struct EntityPosition {
        int id;
        float x;
        float y;
        int score;
        int health;
    };

    explicit JsonParser(std::string_view jsonData) : data(jsonData) {}

    std::vector<Room> parseRooms();
    std::vector<std::string> parsePlayers();
    std::vector<PlayerUDPposition> parsePlayerUDPposition();
    std::vector<EnemyUDPposition> parseEnemyUDPposition();
    std::vector<Leaderboard> parseLeaderboard();
    std::vector<EntityPosition> parseEntityPositions();
    bool validatePlayerPosition(const PlayerUDPposition& pos);

private:
    std::string_view data;
};
```

### ECS (Entity Component System)

The client uses an ECS architecture to manage the game entities. The main ECS components are:

    - Entity: Represents a game entity.
    - Component: Represents a component attached to an entity (e.g., PositionComponent, RenderComponent).
    - System: Contains the logic that operates on components (e.g., MovementSystem, RenderSystem).
    - ParallaxBackground: Handles the display of the parallax background, creating a depth effect by moving multiple background layers at different speeds.

### ECS World (World.hpp)

The `World` class serves as the main container and manager for the Entity Component System (ECS). It manages all entities and systems, coordinates their interactions, and handles the game's parallax background.

#### Class Structure
```cpp
class World {
private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::unique_ptr<System>> systems;
    std::unique_ptr<ParallaxBackground> background;
    sf::RenderWindow& window;
```

#### Key Features

1. **Entity Management**:
- `addEntity()`: Adds new entities to the world
- `removeEntity()`: Removes entities from the world
- `findEntityWithComponent<T>()`: Finds entities with specific components
- `findEntityById()`: Retrieves entities by their unique ID
2. **System Management**:
- `addSystem()`: Adds new systems to process entities
- Template-based system creation for flexibility
- Supports dynamic system type casting for specific system updates
3. **Update and Render Cycle**:
- `update()`: Updates all systems with the current delta time
- `render()`: Manages the rendering pipeline including background and entities
- Handles specific system updates for rendering and score display
4. Background Management:
- Integrates with ParallaxBackground for seamless scrolling effects
- Updates and renders background independently of entity systems

#### Usage Example
```cpp
// Create a world instance
sf::RenderWindow window(sf::VideoMode(800, 600), "R-Type");
World gameWorld(window);

// Add systems
gameWorld.addSystem<MovementSystem>();
gameWorld.addSystem<RenderSystem>(window);

// Add entities
auto player = std::make_shared<Entity>();
gameWorld.addEntity(player);

// Main game loop
while (window.isOpen()) {
    gameWorld.update(deltaTime);
    gameWorld.render();
}
```

#### Key Methods

1. **Constructor**:
```cpp
World(sf::RenderWindow& window) : window(window) {
    background = std::make_unique<ParallaxBackground>(window);
    addSystem();
}
```
2. **Entity Management**:
```cpp
void addEntity(std::shared_ptr<Entity> entity);
void removeEntity(std::shared_ptr<Entity> entity);
```
3. **System Management**:
```cpp
template<typename T, typename... Args>
void addSystem(Args&&... args);
```
4. **Update and Render**:
```cpp
void update(float deltaTime = 0.0f);
void render();
```
5. **Entity Queries**:
```cpp
template<typename T>
std::shared_ptr<Entity> findEntityWithComponent();
std::shared_ptr<Entity> findEntityById(int id);
```

#### Design Principles

The World class follows several key design principles:

1. **Encapsulation**:
- Manages all ECS components in a centralized location
- Provides clear interfaces for entity and system management
2. **Flexibility**:
- Template-based system creation
- Component-based entity queries
- Support for various entity types and systems
3. **Performance**:
- Efficient entity storage using shared pointers
- Optimized rendering pipeline
- Smart pointer usage for automatic resource management
4. **Maintainability**:
- Clear separation of concerns
- Structured update and render cycles
- Easy system addition and removal

### System.hpp

The `System.hpp` file defines the base System class and its derived systems that operate on entities in the game. Each system implements specific game logic for different aspects like movement, collision, rendering, etc.

#### Base System Class
```cpp
class System {
    virtual ~System() = default;
    virtual void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime = 0.0f) = 0;
};
```

#### Key Systems

1. **MovementSystem**
- Updates entity positions based on velocity components
- Handles boundary checking to keep entities within screen limits
2. **RenderSystem**
- Manages rendering of all entities to the SFML window
- Converts game coordinates to screen coordinates
- Handles sprite positioning and drawing
3. **CollisionSystem**
- Detects and handles collisions between different entities
- Manages collision effects (damage, destruction, etc.)
- Creates explosion effects on collisions
- Updates score when appropriate
4. **BossSystem**
- Controls boss entity behavior and movement patterns
- Manages boss attack patterns and projectiles
- Handles boss health and state
5. **ScoreRenderSystem**
- Displays and updates the player's score
- Manages score text positioning and font rendering
6. ExplosionSystem
- Handles explosion animations and lifecycle
- Removes expired explosion entities
7. **PowerUpSystem**
- Controls power-up spawning and movement
- Manages power-up collection and effects
- Handles power-up duration tracking
8. **ObstacleSystem**
- Manages obstacle movement and collision detection
- Handles player-obstacle interaction
- Controls obstacle spawning and despawning
9. MovementPatternSystem
- Implements different movement patterns for enemies
- Supports linear, sinusoidal, and circular movement
- Uses time-based movement calculations

Each system follows these key principles:
- Updates only entities with relevant components
- Handles specific game mechanics independently
- Processes entities in a vectorized way for better performance
- Uses delta time for frame-rate independent updates

Example usage:
```cpp
class Game {
    std::vector<std::unique_ptr<System>> systems;
    std::vector<std::shared_ptr<Entity>> entities;

    void initialize() {
        systems.push_back(std::make_unique<MovementSystem>());
        systems.push_back(std::make_unique<CollisionSystem>(networkManager));
        systems.push_back(std::make_unique<RenderSystem>(window));
        // Add other systems...
    }

    void update(float deltaTime) {
        for (auto& system : systems) {
            system->update(entities, deltaTime);
        }
    }
};
```

The systems architecture allows for easy addition of new game mechanics by creating new system classes that inherit from the base System class.

### ParallaxBackground

The `ParallaxBackground.hpp` file implements a parallax scrolling background effect that creates a sense of depth and movement in the game. The class manages multiple background layers moving at different speeds and includes a star field effect for additional visual depth.

Key features include:

- Multiple background layers with varying speeds and transparencies
- Automated scrolling of layers
- Dynamic star field generation and animation
- Smooth infinite scrolling effect

Here's the structure and main components:
```cpp
class ParallaxBackground {
private:
    struct Layer {
        sf::Sprite sprite;
        float speed;
        float position;
    };

    struct Star {
        sf::CircleShape shape;
        float speed;
        float x, y;
    };
    
    sf::RenderWindow& window;
    std::vector<Layer> layers;
    std::vector<Star> stars;
    std::shared_ptr<sf::Texture> bgTexture;
```

The class implementation includes several key methods:

1. **Constructor**:
- Loads the background texture from "client/sprites/bg-stage1.jpg"
- Creates three parallax layers with different speeds (1.0f, 1.5f, 2.0f)
- Initializes the star field
2. **update()**:
- Updates the position of each background layer
- Manages layer wraparound for infinite scrolling
- Updates star positions with different velocities
3. **render()**:
- Draws background layers in order from back to front
- andles seamless texture repetition
- Renders the star field
4. **Private Helper Methods**:
- `createLayer(float speed)`: Creates a new background layer with specified speed and transparency
- `initializeStars()`: Generates random stars with varying sizes, speeds, and positions

The parallax effect is achieved by:

- Moving multiple copies of the same background at different speeds
- Adjusting layer transparency based on speed (faster layers are more opaque)
- Adding a moving star field with randomized properties
- Using texture repetition for seamless scrolling

Usage example:
```cpp
sf::RenderWindow window(sf::VideoMode(800, 600), "R-Type");
ParallaxBackground background(window);

// In the game loop:
background.update();
background.render();
```

### Entity.hpp

The `Entity.hpp` file defines the `Entity` class, which is a core part of the ECS architecture. The `Entity` class is used to represent a game entity in the system. It holds components and provides methods to manage their lifecycle. Here is an overview of the `Entity` class:

```cpp
#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "Component.hpp"

class Entity {
private:
    int id = -1;
    int netId = -1;
    bool hasValidId = false;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
    bool markForDeletion = false;

public:
    void setId(int newId) {
        id = newId;
        hasValidId = true;
    }

    void setNetId(int newNetId) {
        netId = newNetId;
    }
    
    bool hasId() const {
        return hasValidId;
    }
    
    int getId() const {
        if (!hasValidId) {
            return -1;
        }
        return id;
    }

    int getNetId() const {
        return netId;
    }

    void markForDelete() { markForDeletion = true; }
    bool isMarkedForDeletion() const { return markForDeletion; }

    template<typename T>
    void addComponent(T component) {
        components[std::type_index(typeid(T))] = std::make_shared<T>(component);
    }

    template<typename T, typename... Args>
    void addComponent(Args&&... args) {
        components[std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    bool hasComponent() const {
        return components.find(std::type_index(typeid(T))) != components.end();
    }

    template<typename T>
    std::shared_ptr<T> getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }
};
```

This `Entity` class provides functionality such as:

- **Adding and managing components**: Entities can have various components (e.g., position, render) added to them at runtime.
- **Checking for components**: You can query whether an entity has a specific component attached.
- **Managing entity lifecycle**: The class includes methods to set IDs, mark entities for deletion, and check if an entity has a valid ID.

The use of `std::unordered_map` with `std::type_index` allows for efficient storage and retrieval of components associated with entities.

### EnemyFactory

The `EnemyFactory.hpp` file defines the `EnemyFactory` class that creates enemies of various types. It provides a method to generate enemies with different attributes, such as movement patterns, textures, and animation frames. The class supports creating different types of enemies, including basic enemies, fast enemies, tank enemies, and boss enemies with varying characteristics.

The `EnemyFactory` class works as follows:

1. It creates a new enemy entity.
2. It adds components such as `PositionComponent`, `RenderComponent`, and `MovementPatternComponent`.
3. It configures the render component (`RenderComponent`) for the enemy based on the type, including texture, frame size, scaling, and animation settings.
4. The appropriate texture for the enemy is loaded using the `TextureManager::loadTexture` function.
5. The enemy entity is returned with the configured components.

This design allows for flexible creation of different types of enemies with varying characteristics, enhancing the gameplay experience by providing diverse enemy behavior and appearance.

Example:
```cpp
std::shared_ptr<Entity> basicEnemy = EnemyFactory::createEnemy(EnemyFactory::EnemyType::BASIC, 100.0f, 150.0f);
```

#### Enemy Types Supported:

- **BASIC**: A basic enemy type with standard movement and a simple sprite animation.
- **FAST**: A fast-moving enemy with a sinusoidal movement pattern.
- **TANK**: A tank-like enemy with higher health and more durability.
- **BOSS_TYPE_1, BOSS_TYPE_2, BOSS_TYPE_3**: Different boss enemies with more complex movement patterns and larger sprites.

### ObstacleFactory

The `ObstacleFactory.hpp` file defines the `ObstacleFactory` class responsible for creating obstacle entities in the game. This factory creates obstacles with specific properties and animations that can affect player movement and gameplay mechanics.

The `ObstacleFactory` class provides the following functionality:
```cpp
class ObstacleFactory {
public:
    static std::shared_ptr<Entity> createObstacle(float x, float y) {
        auto obstacle = std::make_shared<Entity>();
        obstacle->addComponent<PositionComponent>(x, y);
        obstacle->addComponent<ObstacleComponent>(0.01f);
        
        auto render = RenderComponent(RenderComponent::Type::OBSTACLE);
        
        // Load the obstacle sprite sheet
        render.texture = TextureManager::loadTexture("client/sprites/r-typesheet39.png");
        render.sprite.setTexture(*render.texture);
        
        // Configure animation
        render.frameRect = sf::IntRect(0, 0, 32, 100);
        render.sprite.setTextureRect(render.frameRect);
        render.sprite.setOrigin(16.0f, 90.0f);
        render.sprite.setScale(4.0f, 4.0f);
        
        // Animation settings
        render.frameTime = 0.08f;
        render.frameCount = 3;
        render.currentFrame = 0;
        render.animationTimer = 0.0f;
        render.frameWidth = 32;
        
        obstacle->addComponent<RenderComponent>(render);
        return obstacle;
    }
};
```

Key features of the ObstacleFactory include:

- Creation of obstacle entities with specific positions
- Addition of obstacle-specific components (PositionComponent and ObstacleComponent)
- Configuration of rendering properties including animations
- Sprite sheet management for obstacle appearances
- Scale and origin point settings for proper obstacle placement

### SoundManager

The `SoundManager.hpp` file defines a static class that manages all sound effects in the game. It provides centralized sound management with features for loading, playing, and controlling sound volume.

Key features include:

- Global volume control for all active sounds
- Efficient sound buffer management using shared pointers
- Automatic tracking of active sound instances
- Thread-safe sound playback

Here's the structure and main components:
```cpp
class SoundManager {
private:
    static std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers;
    static std::vector<std::shared_ptr<sf::Sound>> activeSounds;
    static float globalVolume;

public:
    static std::shared_ptr<sf::Sound> playSound(const std::string& filename);
    static void setGlobalVolume(float volume);
    static float getGlobalVolume();
};
```

The class provides the following functionality:
1. **Sound Playback**:
- `playSound(const std::string& filename)`: Plays a sound file and returns a handle to the sound instance
Automatically manages sound buffer loading and caching
2. **Volume Control**:
- `setGlobalVolume(float volume)`: Sets the volume for all active sounds
- `getGlobalVolume()`: Retrieves the current global volume setting
- Volume changes affect all currently playing and future sounds
3. **Resource Management**:
- Efficient memory usage through shared sound buffers
- Automatic cleanup of stopped sounds
- Prevention of memory leaks through smart pointer usage

Usage example:
```cpp
// Play a sound effect
auto sound = SoundManager::playSound("explosion.wav");

// Adjust global volume (0.0f to 100.0f)
SoundManager::setGlobalVolume(75.0f);

// Get current volume setting
float currentVolume = SoundManager::getGlobalVolume();
```

### TextureManager

The `TextureManager` class provides centralized texture management for the game, implementing a singleton pattern to efficiently handle texture loading and caching.

#### Class Definition
```cpp
// TextureManager.hpp
class TextureManager {
private:
    static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;

public:
    static std::shared_ptr<sf::Texture> loadTexture(const std::string& path);
};
```

#### Implementation
```cpp
// TextureManager.cpp
std::unordered_map<std::string, std::shared_ptr<sf::Texture>> TextureManager::textures;
```

The TextureManager provides the following functionality:

1. **Texture Loading Method**:
```cpp
static std::shared_ptr<sf::Texture> loadTexture(const std::string& path)
```
- Checks if texture exists in cache
- If found, returns cached texture
- If not found, loads texture from file
- Stores new texture in cache
- Returns nullptr if loading fails
2. **Storage**:
- Uses static unordered_map for texture cache
- Keys are file paths (strings)
- Values are shared pointers to sf::Texture objects

Key features:

- **Singleton Pattern**: Ensures only one instance of texture storage exists
- **Lazy Loading**: Textures are loaded only when first requested
- **Memory Management**: Uses shared pointers for automatic resource cleanup
- **Caching**: Prevents redundant loading of the same texture
- **Thread Safety**: Can be made thread-safe with appropriate mutex implementations

Sample usage:
```cpp
// Loading a texture
auto texture = TextureManager::loadTexture("client/sprites/player.png");
if (texture) {
    // Using the texture with a sprite
    sf::Sprite sprite;
    sprite.setTexture(*texture);
} else {
    // Handle loading error
}
```

Benefits:

1. **Performance Optimization**:
- Reduces memory usage by preventing duplicate textures
- Faster access to frequently used textures
- Efficient resource management through shared pointers
2. **Resource Management**:
- Automatic cleanup when textures are no longer needed
- Centralized error handling for texture loading
- Easy texture reuse across multiple sprites
3. **Maintainability**:
- Single point of control for texture loading
- Simplified texture management throughout the game
- Easy to implement additional features like texture reloading or hot-swapping

Error Handling:

- Returns nullptr if texture loading fails
- Allows for graceful handling of missing or corrupt texture files
- Prevents crashes from invalid texture references

### Component.hpp

The `Component.hpp` file defines various components used in the ECS architecture to represent the state and behavior of entities in the game. Each component class inherits from the `Component` base class and serves a specific purpose. The following components are defined:

1. **PositionComponent**: Represents the position of an entity with `x` and `y` coordinates.
2. **VelocityComponent**: Represents the velocity of an entity, with `dx` and `dy` values for movement.
3. **RenderComponent**: Handles rendering properties, including the entity's texture, sprite, frame animation, and rotation.
4. **PlayerComponent**: Defines player-specific data, such as animation state and abilities.
5. **BulletComponent**: Represents the properties of a bullet.
6. **EnemyComponent**: Defines enemy behavior and properties.
7. **BossComponent**: Represents boss entities with their specific properties.
8. **ScoreComponent**: Holds the player's score.
9. **ExplosionComponent**: Handles explosions and their lifecycle.
10. **MovementPatternComponent**: Represents different movement patterns.
11. **PowerUpComponent**: Defines power-up types and effects.
12. **ObstacleComponent**: Represents obstacles that affect entity movement.

### Event Handling
The Events.cpp file manages user events, such as mouse clicks and key presses, and passes them to the appropriate ECS systems for updates.

### ECS Systems

The main ECS systems used in the client are:
```
- MovementSystem: Updates the positions of entities based on their velocities.
- RenderSystem: Draws the entities on the screen.
- CollisionSystem: Checks for collisions between entities and applies the appropriate effects.
- BossSystem: Manages the behavior of bosses in the game.
- ScoreRenderSystem: Displays the players' scores on the screen.
- ExplosionSystem: Handles explosions in the game.
- PowerUpSystem: Manages power-up bonuses in the game.
```

### BossFactory

The `BossFactory.hpp` file defines the `BossFactory` class that creates bosses for various game stages. It defines different types of bosses and configures their components, including position, velocity, rendering, and animation. The class supports creating bosses for stages 1 to 5, with increasing size and complexity.

Example:
```cpp
std::shared_ptr<Entity> boss = BossFactory::createBoss(BossFactory::BossType::STAGE1);
```

## Conclusion

The R-Type game client is a complex application that uses SFML for graphical display and Boost for managing threads and network operations. The ECS architecture allows for modular and extensible management of game entities. This documentation provides an overview of the main components and systems used in the client.