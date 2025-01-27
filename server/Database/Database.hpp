#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
#include <algorithm>

class Database {
    public:
        Database();
        ~Database() = default;

        void writeInDatabase(const std::string &filename, const std::string &message);
        void readDatabaseFile(const std::string &filename);
        
        
        // getter
        void getAllInfoFromId(int id);

        std::string getUsernameFromId(int id);
        int getIdFromUsernameAndPassword(const std::string &username, const std::string &password);
        int getIdFromUsername(const std::string &username);
        void getPasswordFromUsername(const std::string &username);
        void getPasswordFromId(int id);
        void getTcpSocketFromId(int id);
        void getUdpSocketFromId(int id);
        int getLastId();
        std::string getLeaderboard(const std::string &filename);
        int getScoreFromId(const std::string &filename, int id);
        int getNbrKillFromId(const std::string &filename, int id);
        int getNbrDeathFromId(const std::string &filename, int id);
        int getNbrGameFromId(const std::string &filename, int id);
        int getNbrWinFromId(const std::string &filename, int id);
        int getNbrLoseFromId(const std::string &filename, int id);

        std::string getStatistics(const std::string &filename);
        std::string getStatisticsFromId(const std::string &filename, int id);

        // setter
        void setUserId(int id);
        void setUsername(const std::string &username);
        void setPassword(const std::string &password);
        void setAdmin(const bool admin);
        void setBan(const bool ban);
        void setTcpSocket(const std::string &tcp_socket);
        void setUdpSocket(const std::string &udp_socket);
        void setScoreFromId(const std::string &filename, int id, int score);
        void setNbrKillFromId(const std::string &filename, int id, int nbr_kill);
        void setNbrDeathFromId(const std::string &filename, int id, int nbr_death);
        void setNbrGameFromId(const std::string &filename, int id, int nbr_game);
        void setNbrWinFromId(const std::string &filename, int id, int nbr_win);
        void setNbrLoseFromId(const std::string &filename, int id, int nbr_lose);


        void setUsernameFromId(int id, const std::string &username);
        void setPasswordFromId(int id, const std::string &password);
        void setAdminFromId(int id, bool admin);
        void setBanFromId(int id, bool ban);
        void setTcpSocketFromId(int id, const std::string &tcp_socket);
        void setUdpSocketFromId(int id, const std::string &udp_socket);
        int setNewScoreInLeaderBoard(const std::string &filename, const std::string &username, int score);
        void setStatsFromId(const std::string &filename, int id, int score, int nbr_kill, int nbr_death, int nbr_game, int nbr_win, int nbr_lose);

        // check
        bool checkIfPlayerAlreadyExist(const std::string &username, const std::string &password);
        bool checkIfUsernameAlreadyTaken(const std::string &username);
        void checkAndCreateDatabaseFile(const std::string &filename);
        bool checkIfPlayerIsBanned(const std::string &username);
        bool checkIfPlayerIsAdmin(const std::string &username);

        // dump
        void dumpDatabase();
    private:
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> database_;
        int user_id_ = 0;
        std::string username_;
        std::string password_;
        std::string tcp_socket_;
        std::string udp_socket_;
};

#endif // DATABASE_HPP