#ifndef PLAYER_H
#define PLAYER_H

#include "Board.h"
#include <string>

class Player {
protected:
    std::string name;
    char symbol;

public:
    Player(const std::string& playerName, char playerSymbol);
    virtual ~Player() = default;
    
    virtual void getMove(int& x, int& y, const Board& board) = 0;  // 纯虚函数
    
    std::string getName() const;
    char getSymbol() const;
};

// 创建具体的人类玩家类
class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name, char symbol);
    void getMove(int& x, int& y, const Board& board) override;
};

#endif