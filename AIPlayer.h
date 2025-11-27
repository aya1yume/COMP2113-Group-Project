#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"

class AIPlayer : public Player {
private:
    int difficulty;

public:
    AIPlayer(const std::string& name, char symbol, int diff);
    void getMove(int& x, int& y, const Board& board) override;
    
private:
    void easyMove(int& x, int& y, const Board& board);
    void mediumMove(int& x, int& y, const Board& board);
    void hardMove(int& x, int& y, const Board& board);
    
    // 辅助方法
    bool findWinningMove(int& x, int& y, const Board& board, char playerSymbol);
    bool findStrategicMove(int& x, int& y, const Board& board, char playerSymbol);
    bool countThreat(int x, int y, const Board& board, char playerSymbol, int targetCount);
    int evaluatePosition(int x, int y, const Board& board, char mySymbol, char opponentSymbol);
    int evaluateThreat(int x, int y, const Board& board, char playerSymbol);
    int evaluatePatterns(int x, int y, const Board& board, char mySymbol, char opponentSymbol);
};

#endif