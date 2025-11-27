#include "Board.h"
#include <iostream>
#include <iomanip>

using namespace std;

Board::Board(int boardSize, int winCond) : size(boardSize), winCondition(winCond) {
    grid.resize(size, vector<char>(size, ' '));
}

// 赋值操作符实现
Board& Board::operator=(const Board& other) {
    if (this != &other) {  // 防止自我赋值
        size = other.size;
        winCondition = other.winCondition;
        grid = other.grid;  // vector 的赋值操作会进行深拷贝
    }
    return *this;
}

bool Board::placeStone(int x, int y, char playerSymbol) {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        return false;
    }
    if (grid[x][y] != ' ') {
        return false;
    }
    grid[x][y] = playerSymbol;
    return true;
}

bool Board::checkWin(int x, int y, char playerSymbol) {
    // 定义8个方向：水平、垂直、两个对角线（每个方向有正反两个）
    int directions[4][2] = {
        {1, 0},  // 水平 →
        {0, 1},  // 垂直 ↓
        {1, 1},  // 对角线 ↘
        {1, -1}  // 对角线 ↙
    };
    
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int count = 1;  // 当前位置已经有一个棋子
        
        // 正向检查
        for (int step = 1; step < winCondition; step++) {
            int newX = x + dx * step;
            int newY = y + dy * step;
            
            if (newX >= 0 && newX < size && newY >= 0 && newY < size && 
                grid[newX][newY] == playerSymbol) {
                count++;
            } else {
                break;
            }
        }
        
        // 反向检查
        for (int step = 1; step < winCondition; step++) {
            int newX = x - dx * step;
            int newY = y - dy * step;
            
            if (newX >= 0 && newX < size && newY >= 0 && newY < size && 
                grid[newX][newY] == playerSymbol) {
                count++;
            } else {
                break;
            }
        }
        
        // 如果连续棋子数达到获胜条件，返回true
        if (count >= winCondition) {
            return true;
        }
    }
    
    return false;
}

bool Board::isFull() const {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

void Board::display() const {
    // 显示列号（十六进制）- 使用格式化确保对齐
    cout << "    ";  // 行号区域占3个字符宽度
    for (int j = 0; j < size; j++) {
        cout << setw(2) << left;  // 设置宽度为2，左对齐
        if (j < 10) {
            cout << j;
        } else {
            cout << char('A' + (j - 10));
        }
    }
    cout << endl;
    
    // 显示顶部边框
    cout << "  +";
    for (int j = 0; j < size; j++) {
        cout << "--";
    }
    cout << "-+" << endl;
    
    // 显示棋盘内容
    for (int i = 0; i < size; i++) {
        // 显示行号 - 固定宽度
        cout << setw(2) << left;
        if (i < 10) {
            cout << i;
        } else {
            cout << char('A' + (i - 10));
        }
        cout << "|";
        
        // 显示棋盘内容 - 每个位置固定为2字符宽度
        for (int j = 0; j < size; j++) {
            cout << " " << grid[i][j];
        }
        cout << " |" << endl;
    }
    
    // 显示底部边框
    cout << "  +";
    for (int j = 0; j < size; j++) {
        cout << "--";
    }
    cout << "-+" << endl;
}

char Board::getCell(int x, int y) const {
    if (x >= 0 && x < size && y >= 0 && y < size) {
        return grid[x][y];
    }
    return '?';
}

int Board::getSize() const {
    return size;
}

void Board::clear() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            grid[i][j] = ' ';
        }
    }
}