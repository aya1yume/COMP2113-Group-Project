#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>

using namespace std;

class Board {
private:
    vector<vector<char>> grid;  // 15x15的棋盘网格
    int size;                   // 棋盘大小
    int winCondition;           // 获胜条件（五子棋为5）

public:
    // 构造函数：初始化棋盘大小和获胜条件
    Board(int boardSize = 15, int winCond = 5);
    
    // 赋值操作符
    Board& operator=(const Board& other);
    
    // 在指定位置放置棋子，返回是否成功
    bool placeStone(int x, int y, char playerSymbol);
    
    // 检查从(x,y)位置开始是否有玩家获胜
    bool checkWin(int x, int y, char playerSymbol);
    
    // 检查棋盘是否已满（平局）
    bool isFull() const;
    
    // 显示当前棋盘状态
    void display() const;
    
    // 调试显示函数：直接输出所有非空格子的位置
    void debugDisplay() const;
    
    // 获取指定位置的棋子
    char getCell(int x, int y) const;
    
    // 获取棋盘大小
    int getSize() const;
    
    // 清空棋盘（重新开始）
    void clear();
};

#endif