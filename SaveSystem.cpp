#include "SaveSystem.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

// 检查文件是否存在
bool SaveSystem::saveExists(const string& filename) {
    ifstream file(filename);
    bool exists = file.good();
    file.close();
    return exists;
}

// 检查文件是否是有效的存档文件
bool SaveSystem::isValidSaveFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;
    
    string firstLine;
    getline(file, firstLine);
    file.close();
    
    return firstLine == "FIVE_IN_A_ROW_SAVE_V2";
}

// 列出所有可用的存档文件
vector<string> SaveSystem::listSaveFiles() {
    vector<string> saveFiles;
    
    // 检查一些常见的存档文件名
    vector<string> possibleFiles = {
        "game1.save", "game2.save", "game3.save", 
        "autosave.save", "savegame.save", "backup.save"
    };
    
    // 添加用户可能使用的其他文件名
    for (int i = 1; i <= 10; i++) {
        possibleFiles.push_back("save" + to_string(i) + ".save");
    }
    
    for (const string& filename : possibleFiles) {
        if (saveExists(filename) && isValidSaveFile(filename)) {
            saveFiles.push_back(filename);
        }
    }
    
    return saveFiles;
}

bool SaveSystem::saveGame(const string& filename, const Board& board, 
                         const string& currentPlayer, int difficulty) {
    // 确保文件名有正确的扩展名
    string actualFilename = filename;
    if (actualFilename.find(".") == string::npos) {
        actualFilename += ".save";
    }
    
    ofstream file(actualFilename);
    if (!file.is_open()) {
        cout << "Error: Cannot create save file '" << actualFilename << "'!" << endl;
        return false;
    }
    
    // 新的存档格式：记录每个棋子的位置
    file << "FIVE_IN_A_ROW_SAVE_V2" << endl;
    file << "CURRENT_PLAYER:" << currentPlayer << endl;
    file << "DIFFICULTY:" << difficulty << endl;
    file << "BOARD_SIZE:" << board.getSize() << endl;
    
    // 记录所有棋子的位置
    file << "MOVES:" << endl;
    int moveCount = 0;
    for (int i = 0; i < board.getSize(); i++) {
        for (int j = 0; j < board.getSize(); j++) {
            char cell = board.getCell(i, j);
            if (cell != ' ') {
                file << i << " " << j << " " << cell << endl;
                moveCount++;
            }
        }
    }
    file << "END_MOVES" << endl;
    file << "TOTAL_MOVES:" << moveCount << endl;
    
    file.close();
    cout << "Game saved successfully to '" << actualFilename << "' with " << moveCount << " moves" << endl;
    return true;
}

bool SaveSystem::loadGame(const string& filename, Board& board, 
                         string& currentPlayer, int& difficulty) {
    // 确保文件名有正确的扩展名
    string actualFilename = filename;
    if (actualFilename.find(".") == string::npos) {
        actualFilename += ".save";
    }
    
    ifstream file(actualFilename);
    if (!file.is_open()) {
        cout << "Error: Cannot open save file '" << actualFilename << "'!" << endl;
        return false;
    }
    
    string line;
    
    // 检查文件格式
    getline(file, line);
    if (line != "FIVE_IN_A_ROW_SAVE_V2") {
        cout << "Error: Invalid save file format! Expected FIVE_IN_A_ROW_SAVE_V2 but got: " << line << endl;
        file.close();
        return false;
    }
    
    // 读取当前玩家
    getline(file, line);
    currentPlayer = line.substr(line.find(":") + 1);
    
    // 读取难度
    getline(file, line);
    difficulty = stoi(line.substr(line.find(":") + 1));
    
    // 读取棋盘大小
    getline(file, line);
    int boardSize = stoi(line.substr(line.find(":") + 1));
    
    // 创建新棋盘
    board = Board(boardSize);
    
    // 跳过"MOVES:"行
    getline(file, line);
    
    // 读取并放置所有棋子
    int movesLoaded = 0;
    while (getline(file, line)) {
        if (line == "END_MOVES") {
            break;
        }
        
        // 解析棋子位置：x y symbol
        istringstream iss(line);
        int x, y;
        char symbol;
        
        if (iss >> x >> y >> symbol) {
            if (board.placeStone(x, y, symbol)) {
                movesLoaded++;
            } else {
                cout << "Warning: Failed to place stone at (" << x << ", " << y << ") with symbol '" << symbol << "'" << endl;
            }
        } else {
            cout << "Warning: Invalid move format: " << line << endl;
        }
    }
    
    // 读取总步数（用于验证）
    getline(file, line);
    int totalMoves = 0;
    if (line.find("TOTAL_MOVES:") != string::npos) {
        totalMoves = stoi(line.substr(line.find(":") + 1));
    }
    
    file.close();
    
    cout << "Game loaded successfully from '" << actualFilename << "'" << endl;
    cout << "Loaded " << movesLoaded << " moves" << endl;
    
    return true;
}