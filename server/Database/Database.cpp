#include "Database.hpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;

Database::Database()
{
    checkAndCreateDatabaseFile("server/Database/table/users.json");
    checkAndCreateDatabaseFile("server/Database/table/leaderboards.json");
    checkAndCreateDatabaseFile("server/Database/table/statistic.json");
}

void Database::readDatabaseFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    json j;
    file >> j;

    for (auto& [id, user] : j.items()) {
        std::string current_id = id;
        for (auto& [key, value] : user.items()) {
            if (key == "id" || key == "username" || key == "password" || key == "tcp_socket" || key == "udp_socket") {
                database_[current_id][key] = value.get<std::string>();
            }
        }
    }

    file.close();
    std::cout << "Database file " << filename << " read successfully." << std::endl;
}

void Database::writeInDatabase(const std::string &filename, const std::string &message)
{
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        std::cerr << "Full path attempted: " << std::filesystem::absolute(filename) << std::endl;
        return;
    }

    file << message << std::endl;
    file.close();
    std::cout << "Message written in "<< filename << std::endl;
}

std::string Database::getUsernameFromId(int id)
{
    std::string filename = "server/Database/table/users.json";

    readDatabaseFile(filename);

    std::string id_str = std::to_string(id);
    if (database_.find(id_str) != database_.end()) {
        std::cout << "Username: " << database_[id_str]["username"] << std::endl;
        return database_[id_str]["username"];
    } else {
        std::cerr << "Error: ID not found" << std::endl;
        return "";
    }
}

int Database::getIdFromUsernameAndPassword(const std::string &username, const std::string &password)
{
    for (const auto &entry : database_) {
        if (entry.second.at("username") == username && entry.second.at("password") == password) {
            std::cout << "ID: " << entry.first << std::endl;
            return std::stoi(entry.first);
        }
    }
    std::cerr << "Error: Username and password not found" << std::endl;
    return -1;
}

int Database::getIdFromUsername(const std::string &username)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    for (const auto &entry : database_) {
        if (entry.second.at("username") == username) {
            std::cout << "ID: " << entry.first << std::endl;
            return std::stoi(entry.first);
        }
    }
    std::cerr << "Error: Username not found" << std::endl;
    return -1;
}

void Database::getPasswordFromUsername(const std::string &username)
{
    for (const auto &entry : database_) {
        if (entry.second.at("username") == username) {
            std::cout << "Password: " << entry.second.at("password") << std::endl;
            return;
        }
    }
    std::cerr << "Error: Username not found" << std::endl;
}

void Database::getPasswordFromId(int id)
{
    std::string id_str = std::to_string(id);
    if (database_.find(id_str) != database_.end()) {
        std::cout << "Password: " << database_[id_str]["password"] << std::endl;
    } else {
        std::cerr << "Error: ID not found" << std::endl;
    }
}

void Database::getTcpSocketFromId(int id)
{
    std::string id_str = std::to_string(id);
    if (database_.find(id_str) != database_.end()) {
        std::cout << "TCP Socket: " << database_[id_str]["tcp_socket"] << std::endl;
    } else {
        std::cerr << "Error: ID not found" << std::endl;
    }
}

void Database::getUdpSocketFromId(int id)
{
    std::string id_str = std::to_string(id);
    if (database_.find(id_str) != database_.end()) {
        std::cout << "UDP Socket: " << database_[id_str]["udp_socket"] << std::endl;
    } else {
        std::cerr << "Error: ID not found" << std::endl;
    }
}

int Database::getLastId()
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    int last_id = 0;
    for (const auto &entry : database_) {
        int id = std::stoi(entry.second.at("id"));
        if (id > last_id) {
            last_id = id;
        }
    }
    std::cout << "Last ID: " << last_id << std::endl;
    return last_id;
}

void Database::setUserId(int id)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    json j;
    for (const auto& [key, value] : database_) {
        j[key] = value;
    }

    int next_sequential_id = 1;
    while (j.contains(std::to_string(next_sequential_id))) {
        next_sequential_id++;
    }

    std::string id_str = std::to_string(id);
    std::string sequential_id_str = std::to_string(next_sequential_id);
    j[sequential_id_str] = {
        {"id", id_str},
        {"username", ""},
        {"password", ""},
        {"tcp_socket", ""},
        {"udp_socket", ""}
    };

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();

    std::cout << "User with ID " << id << " added to the database with sequential ID " << next_sequential_id << "." << std::endl;
}

void Database::setUsernameFromId(int id, const std::string &username)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    json j;
    for (const auto& [key, value] : database_) {
        j[key] = value;
    }

    bool user_found = false;
    for (auto& [key, value] : j.items()) {
        if (value["id"] == std::to_string(id)) {
            value["username"] = username;
            user_found = true;
            break;
        }
    }

    if (!user_found) {
        std::cerr << "Error: User with ID " << id << " not found" << std::endl;
        return;
    }

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();

    std::cout << "Username for user with ID " << id << " updated to " << username << "." << std::endl;
}

void Database::setPasswordFromId(int id, const std::string &password)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    json j;
    for (const auto& [key, value] : database_) {
        j[key] = value;
    }

    bool user_found = false;
    for (auto& [key, value] : j.items()) {
        if (value["id"] == std::to_string(id)) {
            value["password"] = password;
            user_found = true;
            break;
        }
    }

    if (!user_found) {
        std::cerr << "Error: User with ID " << id << " not found" << std::endl;
        return;
    }

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();

    std::cout << "Password for user with ID " << id << " updated to " << password << "." << std::endl;
}

void Database::setAdminFromId(int id, bool admin)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    json j;
    for (const auto& [key, value] : database_) {
        j[key] = value;
    }

    bool user_found = false;
    for (auto& [key, value] : j.items()) {
        if (value["id"] == std::to_string(id)) {
            value["admin"] = admin ? "true" : "false";
            user_found = true;
            break;
        }
    }

    if (!user_found) {
        std::cerr << "Error: User with ID " << id << " not found" << std::endl;
        return;
    }

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();

    std::cout << "Admin status for user with ID " << id << " updated to " << (admin ? "true" : "false") << "." << std::endl;
}

void Database::setBanFromId(int id, bool ban)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    json j;
    for (const auto& [key, value] : database_) {
        j[key] = value;
    }

    bool user_found = false;
    for (auto& [key, value] : j.items()) {
        if (value["id"] == std::to_string(id)) {
            value["ban"] = ban ? "true" : "false";
            user_found = true;
            break;
        }
    }

    if (!user_found) {
        std::cerr << "Error: User with ID " << id << " not found" << std::endl;
        return;
    }

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();

    std::cout << "Ban status for user with ID " << id << " updated to " << (ban ? "true" : "false") << "." << std::endl;
}

void Database::setTcpSocketFromId(int id, const std::string &tcp_socket)
{
    std::string id_str = std::to_string(id);
    if (database_.find(id_str) != database_.end()) {
        database_[id_str]["tcp_socket"] = tcp_socket;
    } else {
        std::cerr << "Error: ID not found" << std::endl;
    }
}

void Database::setUdpSocketFromId(int id, const std::string &udp_socket)
{
    std::string id_str = std::to_string(id);
    if (database_.find(id_str) != database_.end()) {
        database_[id_str]["udp_socket"] = udp_socket;
    } else {
        std::cerr << "Error: ID not found" << std::endl;
    }
}

bool Database::checkIfPlayerAlreadyExist(const std::string &username, const std::string &password)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    json j;
    for (const auto& [key, value] : database_) {
        j[key] = value;
    }

    for (const auto& [key, value] : j.items()) {
        if (value["username"] == username && value["password"] == password) {
            std::cout << "Player already exists with ID " << value["id"] << std::endl;
            return true;
        }
    }

    std::cerr << "Error: Player not found" << std::endl;
    return false;
}

bool Database::checkIfUsernameAlreadyTaken(const std::string &username)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    json j;
    for (const auto& [key, value] : database_) {
        j[key] = value;
    }

    for (const auto& [key, value] : j.items()) {
        if (value["username"] == username) {
            std::cout << "Username already taken" << std::endl;
            return true;
        }
    }

    std::cerr << "Error: Username not found" << std::endl;
    return false;
}

bool Database::checkIfPlayerIsBanned(const std::string &username)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    json j;
    for (const auto& [key, value] : database_) {
        j[key] = value;
    }

    for (const auto& [key, value] : j.items()) {
        if (value["username"] == username && value["ban"] == "true") {
            std::cout << "Player is banned" << std::endl;
            return true;
        }
    }

    return false;
} 

bool Database::checkIfPlayerIsAdmin(const std::string &username)
{
    std::string filename = "server/Database/table/users.json";
    readDatabaseFile(filename);

    json j;
    for (const auto& [key, value] : database_) {
        j[key] = value;
    }

    for (const auto& [key, value] : j.items()) {
        if (value["username"] == username && value["admin"] == "true") {
            std::cout << "Player is admin" << std::endl;
            return true;
        }
    }

    return false;
}

void Database::dumpDatabase()
{
    for (const auto &entry : database_) {
        std::cout << "ID: " << entry.first << std::endl;
        for (const auto &data : entry.second) {
            std::cout << data.first << ": " << data.second << std::endl;
        }
        std::cout << std::endl;
    }
}

void Database::checkAndCreateDatabaseFile(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::ofstream out_file(filename);
        if (!out_file.is_open()) {
            std::cerr << "Error: Could not create file " << filename << std::endl;
            return;
        }

        json j = json::object();
        out_file << std::setw(4) << j << std::endl;
        out_file.close();
        std::cout << "Database file " << filename << " created successfully." << std::endl;
    } else {
        file.close();
    }
}

std::string Database::getLeaderboard(const std::string &filename)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return "{}";
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_array()) {
        std::cerr << "Error: Expected JSON array in " << filename << std::endl;
        return "{}";
    }

    std::vector<std::pair<std::string, int>> leaderboard;
    for (const auto& entry : j) {
        leaderboard.emplace_back(entry["username"], entry["score"]);
    }

    std::sort(leaderboard.begin(), leaderboard.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });

    json top10 = json::array();
    for (size_t i = 0; i < std::min(leaderboard.size(), size_t(10)); ++i) {
        json entry;
        entry["username"] = leaderboard[i].first;
        entry["score"] = leaderboard[i].second;
        top10.push_back(entry);
    }

    return top10.dump(4);
}

int Database::setNewScoreInLeaderBoard(const std::string &filename, const std::string &username, int score)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return -1;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_array()) {
        std::cerr << "Error: Expected JSON array in " << filename << std::endl;
        return -1;
    }

    bool user_found = false;
    for (auto &entry : j) {
        if (entry["username"] == username) {
            entry["score"] = score;
            user_found = true;
            break;
        }
    }

    if (!user_found) {
        json new_entry;
        new_entry["username"] = username;
        new_entry["score"] = score;
        j.push_back(new_entry);
    }

    std::vector<std::pair<std::string, int>> leaderboard;
    for (const auto &entry : j) {
        leaderboard.emplace_back(entry["username"], entry["score"]);
    }

    std::sort(leaderboard.begin(), leaderboard.end(), [](const auto &a, const auto &b)
              { return a.second > b.second; });

    json sorted_j = json::array();
    int position = 0;
    for (size_t i = 0; i < leaderboard.size(); ++i) {
        json entry;
        entry["username"] = leaderboard[i].first;
        entry["score"] = leaderboard[i].second;
        sorted_j.push_back(entry);
        if (leaderboard[i].first == username) {
            position = i + 1;
        }
    }

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return -1;
    }

    out_file << std::setw(4) << sorted_j << std::endl;
    out_file.close();
    std::cout << "Score added to leaderboard file " << filename << std::endl;

    return position;
}

void Database::setStatsFromId(const std::string &filename, int id, int score, int nbr_kill, int nbr_death, int nbr_game, int nbr_win, int nbr_lose)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return;
    }

    json j;
    file >> j;
    file.close();

    // Vérifier si le JSON est un objet
    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return;
    }

    // Mettre à jour les statistiques de l'utilisateur
    std::string id_str = std::to_string(id);
    j[id_str]["score"] = score;
    j[id_str]["nbrKills"] = nbr_kill;
    j[id_str]["nbrDeaths"] = nbr_death;
    j[id_str]["nbrGames"] = nbr_game;
    j[id_str]["nbrWins"] = nbr_win;
    j[id_str]["nbrLosses"] = nbr_lose;

    // Écrire l'objet JSON mis à jour dans le fichier
    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();
    std::cout << "Stats added to stats file " << filename << std::endl;

    return;
}

int Database::getScoreFromId(const std::string &filename, int id)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return -1;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return -1;
    }

    std::string id_str = std::to_string(id);
    if (j.contains(id_str)) {
        return j[id_str]["score"];
    }
    return -1;
}

int Database::getNbrKillFromId(const std::string &filename, int id)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return -1;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return -1;
    }

    std::string id_str = std::to_string(id);
    if (j.contains(id_str)) {
        return j[id_str]["nbrKills"];
    }
    return -1;
}

int Database::getNbrDeathFromId(const std::string &filename, int id)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return -1;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return -1;
    }

    std::string id_str = std::to_string(id);
    if (j.contains(id_str)) {
        return j[id_str]["nbrDeaths"];
    }
    return -1;
}

int Database::getNbrGameFromId(const std::string &filename, int id)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return -1;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return -1;
    }

    std::string id_str = std::to_string(id);
    if (j.contains(id_str)) {
        return j[id_str]["nbrGames"];
    }
    return -1;
}

int Database::getNbrWinFromId(const std::string &filename, int id)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return -1;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return -1;
    }

    std::string id_str = std::to_string(id);
    if (j.contains(id_str)) {
        return j[id_str]["nbrWins"];
    }
    return -1;
}

int Database::getNbrLoseFromId(const std::string &filename, int id)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return -1;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return -1;
    }

    std::string id_str = std::to_string(id);
    if (j.contains(id_str)) {
        return j[id_str]["nbrLosses"];
    }
    return -1;
}

void Database::setScoreFromId(const std::string &filename, int id, int score)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return;
    }

    std::string id_str = std::to_string(id);
    j[id_str]["score"] = score;

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();
    std::cout << "Score added to stats file " << filename << std::endl;

    return;
}

void Database::setNbrKillFromId(const std::string &filename, int id, int nbr_kill)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return;
    }

    std::string id_str = std::to_string(id);
    j[id_str]["nbrKills"] = nbr_kill;

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();
    std::cout << "NbrKill added to stats file " << filename << std::endl;

    return;
}

void Database::setNbrDeathFromId(const std::string &filename, int id, int nbr_death)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return;
    }

    std::string id_str = std::to_string(id);
    j[id_str]["nbrDeaths"] = nbr_death;

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();

    std::cout << "NbrDeath added to stats file " << filename << std::endl;

    return;
}

void Database::setNbrGameFromId(const std::string &filename, int id, int nbr_game)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return;
    }

    std::string id_str = std::to_string(id);
    j[id_str]["nbrGames"] = nbr_game;

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();

    std::cout << "NbrGame added to stats file " << filename << std::endl;

    return;
}

void Database::setNbrWinFromId(const std::string &filename, int id, int nbr_win)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return;
    }

    std::string id_str = std::to_string(id);
    j[id_str]["nbrWins"] = nbr_win;

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();

    std::cout << "NbrWin added to stats file " << filename << std::endl;

    return;
}

void Database::setNbrLoseFromId(const std::string &filename, int id, int nbr_lose)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: File " << filename << " does not exist." << std::endl;
        return;
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_object()) {
        std::cerr << "Error: Expected JSON object in " << filename << std::endl;
        return;
    }

    std::string id_str = std::to_string(id);
    j[id_str]["nbrLosses"] = nbr_lose;

    std::ofstream out_file(filename);
    if (!out_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    out_file << std::setw(4) << j << std::endl;
    out_file.close();

    std::cout << "NbrLose added to stats file " << filename << std::endl;

    return;
}

std::string Database::getStatistics(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return "";
    }

    json j;
    file >> j;
    file.close();

    std::cout << j.dump(4) << std::endl;

    return j.dump(4);
}

std::string Database::getStatisticsFromId(const std::string &filename, int id)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return "";
    }

    json j;
    file >> j;
    file.close();

    std::string id_str = std::to_string(id);
    if (j.contains(id_str)) {
        json player_stats = j[id_str];
        player_stats["id"] = id_str;
        return player_stats.dump(4);
    }

    return "";
}