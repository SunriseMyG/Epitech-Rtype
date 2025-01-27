#include "Room.hpp"

Room::Room(int id, std::string name, std::string password, State state, int maxPlayers, bool privateRoom)
    : id(id), name(name), password(password), state(state), maxPlayers(maxPlayers), score(0), level(1), entityID(0), enemyDead(0), enemyInLevel(0), levelStartTime(std::chrono::high_resolution_clock::now()), privateRoom(privateRoom) {
}

Room::~Room() {
}

//Party
void Room::startGame() {
    this->state = PLAYING;
}

void Room::endGame() {
    this->state = FINISHED;
}

//Setters
void Room::setPassword(std::string password) {
    this->password = password;
}

void Room::setName(std::string name) {
    this->name = name;
}

void Room::setState(int state) {
    this->state = static_cast<State>(state);
}

void Room::setMaxPlayers(int maxPlayers) {
    this->maxPlayers = maxPlayers;
}

void Room::setScore(int score) {
    this->score = score;
}

void Room::setLevel(int level) {
    this->level = level;
}

void Room::setEnemyDead(int enemyDead) {
    this->enemyDead = enemyDead;
}

void Room::resetLevelStartTime() {
    this->levelStartTime = std::chrono::high_resolution_clock::now();
}

void Room::setEnemyInLevel(int enemyInLevel) {
    this->enemyInLevel = enemyInLevel;
}

//Players
void Room::addPlayer(std::shared_ptr<Player> player) {
    this->players.push_back(player);
}

void Room::removePlayer(std::shared_ptr<Player> player) {
    this->players.erase(std::remove(this->players.begin(), this->players.end(), player), this->players.end());
}

//Getters
std::vector<std::shared_ptr<Player>> Room::getPlayers() {
    return this->players;
}

int Room::getId() {
    return this->id;
}

std::string Room::getName() {
    return this->name;
}

std::string Room::getPassword() {
    return this->password;
}

IRoom::State Room::getState() {
    return this->state;
}

int Room::getMaxPlayers() {
    return this->maxPlayers;
}

int Room::getScore() {
    return this->score;
}

int Room::getLevel() {
    return this->level;
}

int Room::getEnemyDead() {
    return this->enemyDead;
}

std::chrono::time_point<std::chrono::high_resolution_clock> Room::getLevelStartTime() {
    return this->levelStartTime;
}

int Room::getEnemyInLevel() {
    return this->enemyInLevel;
}

//Entities

int Room::generateEntityID() {
    return this->entityID++;
}

std::vector<std::shared_ptr<Entity>> Room::getEntities() {
    return this->entities;
}

void Room::addEntity(std::shared_ptr<Entity> entity) {
    this->entities.push_back(entity);
}

void Room::removeEntity(std::shared_ptr<Entity> entity) {
    this->entities.erase(std::remove(this->entities.begin(), this->entities.end(), entity), this->entities.end());
}

void Room::setEntityToPlayer(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity) {
    this->playerEntities.push_back(std::make_pair(player->getId(), entity));
}

void Room::removeEntityFromPlayer(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity) {
    this->playerEntities.erase(std::remove_if(this->playerEntities.begin(), this->playerEntities.end(), [player, entity](const std::pair<int, std::shared_ptr<Entity>>& pair) {
        return pair.first == player->getId() && pair.second == entity;
    }), this->playerEntities.end());
}

Entity Room::getEntityFromPlayer(int playerId) {
    for (const auto& pair : this->playerEntities) {
        if (pair.first == playerId) {
            return *pair.second;
        }
    }
    return Entity(-1, "none", "none");
}

int Room::getPlayerIdFromEntity(std::shared_ptr<Entity> entity) {
    for (const auto& [playerId, playerEntity] : playerEntities) {
        if (playerEntity == entity) {
            return playerId;
        }
    }
    return -1;
}

IRoom::Difficulty Room::getDifficulty() {
    return this->difficulty_;
}

void Room::setDifficulty(Difficulty difficulty) {
    this->difficulty_ = difficulty;
}

bool Room::isPrivate() {
    return this->privateRoom;
}

void Room::kickPlayer(std::shared_ptr<Player> player) {
    this->removePlayer(player);

    if (this->players.size() == 0) {
        this->state = State::FINISHED;
    }
}

void Room::kickPlayer(int playerId) {
    for (const auto& player : this->players) {
        if (player->getId() == playerId) {
            this->removePlayer(player);
            break;
        }
    }

    if (this->players.size() == 0) {
        this->state = State::FINISHED;
    }
}

void Room::update(float deltaTime) {
    elapsedTime += deltaTime;
}

float Room::getCurrentTime() const {
    return elapsedTime;
}