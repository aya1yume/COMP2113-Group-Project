#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include "AIPlayer.h"
#include "SaveSystem.h"
#include <memory>
#include <string>
#include <vector>

class Game {
private:
    Board board;
    Board lastBoard;  // 新增：存储上一步棋盘状态
    std::unique_ptr<Player> player1;
    std::unique_ptr<Player> player2;
    int currentPlayer;
    int difficulty;
    bool gameOver;
    bool returnToMainMenu;
    bool canUndo;  // 新增：标记是否可以回退

public:
    Game();
    
    void startGame();
    void setDifficulty();
    void gameLoop();
    bool saveGame(const std::string& filename);
    bool loadGame(const std::string& filename);
    
private:
    void switchPlayer();
    bool checkGameEnd(int lastX, int lastY);
    void displayStatus() const;
    void askPlayAgain();
    void restartGame();
    bool confirmExit();
    void undoMove();  // 新增：回退方法
    void saveCurrentState();  // 新增：保存当前状态
};

#endif