#include "Player.h"
#include <iostream>
#include <limits>

using namespace std;

Player::Player(const string& playerName, char playerSymbol) 
    : name(playerName), symbol(playerSymbol) {}

string Player::getName() const {
    return name;
}

char Player::getSymbol() const {
    return symbol;
}

// HumanPlayer 实现
HumanPlayer::HumanPlayer(const string& name, char symbol) 
    : Player(name, symbol) {}

void HumanPlayer::getMove(int& x, int& y, const Board& board) {
    bool validMove = false;
    
    while (!validMove) {
        cout << "Enter your move (row column, e.g., 7 7 or A A): ";
        
        string inputX, inputY;
        cin >> inputX >> inputY;
        
        // 转换输入为数字坐标
        if (inputX.length() == 1 && isalpha(inputX[0])) {
            // 十六进制输入 (A-F)
            x = toupper(inputX[0]) - 'A' + 10;
        } else {
            // 数字输入 (0-9)
            x = stoi(inputX);
        }
        
        if (inputY.length() == 1 && isalpha(inputY[0])) {
            // 十六进制输入 (A-F)
            y = toupper(inputY[0]) - 'A' + 10;
        } else {
            // 数字输入 (0-9)
            y = stoi(inputY);
        }
        
        // 检查坐标是否在棋盘范围内
        int size = board.getSize();
        if (x < 0 || x >= size || y < 0 || y >= size) {
            cout << "Invalid move! Position out of bounds. Please try again." << endl;
            continue;
        }
        
        // 检查位置是否为空
        if (board.getCell(x, y) != ' ') {
            cout << "Invalid move! Position already occupied. Please try again." << endl;
            continue;
        }
        
        // 如果通过了所有检查，移动有效
        validMove = true;
    }
}