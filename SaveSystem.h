#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include "Board.h"
#include <string>
#include <vector>

class SaveSystem {
public:
    static bool saveGame(const std::string& filename, const Board& board, 
                        const std::string& currentPlayer, int difficulty);
    static bool loadGame(const std::string& filename, Board& board, 
                        std::string& currentPlayer, int& difficulty);
    static bool saveExists(const std::string& filename);
    static std::vector<std::string> listSaveFiles();
    
private:
    static bool isValidSaveFile(const std::string& filename);
};

#endif