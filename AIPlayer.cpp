#include "AIPlayer.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

AIPlayer::AIPlayer(const string& name, char symbol, int diff) 
    : Player(name, symbol), difficulty(diff) {
    srand(time(0));
}

void AIPlayer::getMove(int& x, int& y, const Board& board) {
    cout << name << " is thinking..." << endl;
    
    switch(difficulty) {
        case 1:
            easyMove(x, y, board);
            break;
        case 2:
            mediumMove(x, y, board);
            break;
        case 3:
            hardMove(x, y, board);
            break;
        default:
            easyMove(x, y, board);
    }
    
    cout << name << " chooses position: " << x << ", " << y << endl;
}

// 简单难度：完全随机落子
void AIPlayer::easyMove(int& x, int& y, const Board& board) {
    int size = board.getSize();
    vector<pair<int, int>> emptyCells;
    
    // 收集所有空位置
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board.getCell(i, j) == ' ') {
                emptyCells.push_back({i, j});
            }
        }
    }
    
    // 随机选择一个空位置
    if (!emptyCells.empty()) {
        int index = rand() % emptyCells.size();
        x = emptyCells[index].first;
        y = emptyCells[index].second;
    } else {
        x = y = 0;
    }
}

// 中级难度：基础攻防策略
void AIPlayer::mediumMove(int& x, int& y, const Board& board) {
    char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
    
    // 1. 检查AI是否有立即获胜的机会
    if (findWinningMove(x, y, board, symbol)) {
        return;
    }
    
    // 2. 检查是否需要阻止玩家获胜
    if (findWinningMove(x, y, board, opponentSymbol)) {
        return;
    }
    
    // 3. 检查并创建活三或冲四
    if (findStrategicMove(x, y, board, symbol)) {
        return;
    }
    
    // 4. 阻止玩家的活三或冲四
    if (findStrategicMove(x, y, board, opponentSymbol)) {
        return;
    }
    
    // 5. 如果以上都没有，选择中心区域或随机落子
    vector<pair<int, int>> goodMoves;
    int size = board.getSize();
    int center = size / 2;
    
    // 优先选择中心区域
    for (int i = center - 2; i <= center + 2; i++) {
        for (int j = center - 2; j <= center + 2; j++) {
            if (i >= 0 && i < size && j >= 0 && j < size && 
                board.getCell(i, j) == ' ') {
                goodMoves.push_back({i, j});
            }
        }
    }
    
    if (!goodMoves.empty()) {
        int index = rand() % goodMoves.size();
        x = goodMoves[index].first;
        y = goodMoves[index].second;
    } else {
        // 如果中心区域没有空位，随机选择
        easyMove(x, y, board);
    }
}

// 高级难度：使用评估函数
void AIPlayer::hardMove(int& x, int& y, const Board& board) {
    char opponentSymbol = (symbol == 'X') ? 'O' : 'X';
    
    // 1. 检查AI是否有立即获胜的机会
    if (findWinningMove(x, y, board, symbol)) {
        return;
    }
    
    // 2. 检查是否需要阻止玩家获胜
    if (findWinningMove(x, y, board, opponentSymbol)) {
        return;
    }
    
    // 3. 使用评估函数选择最佳位置
    map<pair<int, int>, int> scores;
    int size = board.getSize();
    int bestScore = -1;
    
    // 评估每个空位的得分
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board.getCell(i, j) == ' ') {
                int score = evaluatePosition(i, j, board, symbol, opponentSymbol);
                scores[{i, j}] = score;
                
                if (score > bestScore) {
                    bestScore = score;
                }
            }
        }
    }
    
    // 从最高得分的空位中随机选择一个（避免每次都相同）
    vector<pair<int, int>> bestMoves;
    for (const auto& entry : scores) {
        if (entry.second == bestScore) {
            bestMoves.push_back(entry.first);
        }
    }
    
    if (!bestMoves.empty()) {
        int index = rand() % bestMoves.size();
        x = bestMoves[index].first;
        y = bestMoves[index].second;
    } else {
        // 备用方案
        mediumMove(x, y, board);
    }
}

// 检查是否有立即获胜的机会
bool AIPlayer::findWinningMove(int& x, int& y, const Board& board, char playerSymbol) {
    int size = board.getSize();
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board.getCell(i, j) == ' ') {
                // 模拟在这个位置落子
                Board testBoard = board;
                testBoard.placeStone(i, j, playerSymbol);
                
                // 检查是否获胜
                if (testBoard.checkWin(i, j, playerSymbol)) {
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    }
    
    return false;
}

// 寻找战略性移动（创建或阻止活三、冲四等）
bool AIPlayer::findStrategicMove(int& x, int& y, const Board& board, char playerSymbol) {
    int size = board.getSize();
    
    // 优先寻找可以创建冲四的位置
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board.getCell(i, j) == ' ') {
                if (countThreat(i, j, board, playerSymbol, 4)) {
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    }
    
    // 然后寻找可以创建活三的位置
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board.getCell(i, j) == ' ') {
                if (countThreat(i, j, board, playerSymbol, 3)) {
                    x = i;
                    y = j;
                    return true;
                }
            }
        }
    }
    
    return false;
}

// 计算在某个位置落子后形成的威胁程度
bool AIPlayer::countThreat(int x, int y, const Board& board, char playerSymbol, int targetCount) {
    // 定义8个方向
    int directions[4][2] = {
        {1, 0}, {0, 1}, {1, 1}, {1, -1}
    };
    
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        
        // 检查正反两个方向
        int count = 1;  // 当前位置
        
        // 正向检查
        for (int step = 1; step < 5; step++) {
            int newX = x + dx * step;
            int newY = y + dy * step;
            
            if (newX >= 0 && newX < board.getSize() && newY >= 0 && newY < board.getSize() && 
                board.getCell(newX, newY) == playerSymbol) {
                count++;
            } else {
                break;
            }
        }
        
        // 反向检查
        for (int step = 1; step < 5; step++) {
            int newX = x - dx * step;
            int newY = y - dy * step;
            
            if (newX >= 0 && newX < board.getSize() && newY >= 0 && newY < board.getSize() && 
                board.getCell(newX, newY) == playerSymbol) {
                count++;
            } else {
                break;
            }
        }
        
        if (count >= targetCount) {
            return true;
        }
    }
    
    return false;
}

// 评估位置的得分
int AIPlayer::evaluatePosition(int x, int y, const Board& board, char mySymbol, char opponentSymbol) {
    int score = 0;
    
    // 基础位置分 - 中心位置更优
    int size = board.getSize();
    int center = size / 2;
    int distanceFromCenter = abs(x - center) + abs(y - center);
    score += (size - distanceFromCenter) * 2;
    
    // 进攻得分 - 评估在这个位置落子对AI的益处
    score += evaluateThreat(x, y, board, mySymbol) * 10;
    
    // 防守得分 - 评估在这个位置落子对对手的威胁
    score += evaluateThreat(x, y, board, opponentSymbol) * 8;
    
    // 特殊模式识别
    score += evaluatePatterns(x, y, board, mySymbol, opponentSymbol);
    
    return score;
}

// 评估威胁程度
int AIPlayer::evaluateThreat(int x, int y, const Board& board, char playerSymbol) {
    int threatScore = 0;
    
    // 定义8个方向
    int directions[4][2] = {
        {1, 0}, {0, 1}, {1, 1}, {1, -1}
    };
    
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        
        // 检查正反两个方向
        int consecutive = 1;  // 当前位置
        bool blockedFront = false;
        bool blockedBack = false;
        
        // 正向检查
        for (int step = 1; step < 5; step++) {
            int newX = x + dx * step;
            int newY = y + dy * step;
            
            if (newX >= 0 && newX < board.getSize() && newY >= 0 && newY < board.getSize()) {
                if (board.getCell(newX, newY) == playerSymbol) {
                    consecutive++;
                } else if (board.getCell(newX, newY) != ' ') {
                    blockedFront = true;
                    break;
                } else {
                    break;
                }
            } else {
                blockedFront = true;
                break;
            }
        }
        
        // 反向检查
        for (int step = 1; step < 5; step++) {
            int newX = x - dx * step;
            int newY = y - dy * step;
            
            if (newX >= 0 && newX < board.getSize() && newY >= 0 && newY < board.getSize()) {
                if (board.getCell(newX, newY) == playerSymbol) {
                    consecutive++;
                } else if (board.getCell(newX, newY) != ' ') {
                    blockedBack = true;
                    break;
                } else {
                    break;
                }
            } else {
                blockedBack = true;
                break;
            }
        }
        
        // 根据连续棋子数和阻挡情况评分
        if (consecutive >= 4) {
            threatScore += 1000;  // 冲四
        } else if (consecutive == 3) {
            if (!blockedFront || !blockedBack) {
                threatScore += 100;  // 活三
            } else {
                threatScore += 50;   // 冲三
            }
        } else if (consecutive == 2) {
            if (!blockedFront && !blockedBack) {
                threatScore += 10;   // 活二
            } else {
                threatScore += 5;    // 冲二
            }
        }
    }
    
    return threatScore;
}

// 评估特殊模式
int AIPlayer::evaluatePatterns(int x, int y, const Board& board, char mySymbol, char opponentSymbol) {
    int patternScore = 0;
    
    // 检查双三模式（在两个方向都有活三）
    int liveThreeCount = 0;
    
    // 定义8个方向
    int directions[4][2] = {
        {1, 0}, {0, 1}, {1, 1}, {1, -1}
    };
    
    for (int d = 0; d < 4; d++) {
        int dx = directions[d][0];
        int dy = directions[d][1];
        
        // 模拟落子
        Board testBoard = board;
        testBoard.placeStone(x, y, mySymbol);
        
        // 检查这个方向是否形成活三
        if (countThreat(x, y, testBoard, mySymbol, 3)) {
            liveThreeCount++;
        }
    }
    
    // 双三模式很强大
    if (liveThreeCount >= 2) {
        patternScore += 200;
    }
    
    return patternScore;
}