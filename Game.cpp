#include "Game.h"
#include <iostream>
#include <limits>
#include <memory>
#include <cctype>

using namespace std;

Game::Game() : currentPlayer(1), difficulty(1), gameOver(false), 
               returnToMainMenu(false), canUndo(false) {
    player1 = make_unique<HumanPlayer>("Human", 'X');
    player2 = make_unique<AIPlayer>("Computer", 'O', difficulty);
}

void Game::startGame() {
    cout << "=== Five-in-a-Row Game ===" << endl;
    cout << "Rules: Connect 5 stones in a row to win!" << endl;
    
    // 开始新游戏时清空棋盘
    board.clear();
    canUndo = false;  // 新游戏开始时不能回退
    
    setDifficulty();
    gameLoop();
}

void Game::setDifficulty() {
    cout << "\nSelect difficulty level:" << endl;
    cout << "1. Easy (Random moves)" << endl;
    cout << "2. Medium (Basic defense)" << endl;
    cout << "3. Hard (Advanced strategy)" << endl;
    cout << "Enter choice (1-3): ";
    
    int diff;
    while (!(cin >> diff) || diff < 1 || diff > 3) {
        cout << "Invalid input! Please enter 1, 2, or 3: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    difficulty = diff;
    player2 = make_unique<AIPlayer>("Computer", 'O', difficulty);
    cout << "Difficulty set to level " << diff << endl;
}

// 保存当前棋盘状态
void Game::saveCurrentState() {
    lastBoard = board;  // 使用赋值操作符复制棋盘状态
    canUndo = true;
}

void Game::gameLoop() {
    gameOver = false;
    returnToMainMenu = false;
    int lastX = -1, lastY = -1;
    
    cout << "\nGame started! You are 'X', Computer is 'O'" << endl;
    cout << "Commands during your turn:" << endl;
    cout << "  - Enter coordinates (e.g., 7 7 or A A) to make a move" << endl;
    cout << "  - Type 'save' to save the game" << endl;
    cout << "  - Type 'restart' to restart the game" << endl;
    cout << "  - Type 'undo' to undo your last move" << endl;
    cout << "  - Type 'exit' to return to main menu" << endl;
    
    while (!gameOver && !returnToMainMenu) {
        displayStatus();
        
        Player* current = (currentPlayer == 1) ? player1.get() : player2.get();
        cout << current->getName() << "'s turn (" << current->getSymbol() << ")" << endl;
        
        int x = -1, y = -1;
        
        // 如果是人类玩家，检查特殊命令
        if (currentPlayer == 1) {
            string input;
            cout << "Enter your move or command: ";
            cin >> input;
            
            // 处理特殊命令
            if (input == "save" || input == "SAVE") {
                string filename;
                cout << "Enter save file name: ";
                cin >> filename;
                if (saveGame(filename)) {
                    cout << "Game saved successfully! Continuing..." << endl;
                }
                continue;  // 重新开始循环，不切换玩家
            } else if (input == "restart" || input == "RESTART") {
                restartGame();
                continue;  // 重新开始循环
            } else if (input == "undo" || input == "UNDO") {
                undoMove();
                continue;  // 重新开始循环
            } else if (input == "exit" || input == "EXIT") {
                if (confirmExit()) {
                    returnToMainMenu = true;
                    cout << "Returning to main menu..." << endl;
                } else {
                    cout << "Resuming game..." << endl;
                }
                continue;  // 重新开始循环
            } else {
                // 尝试解析为坐标
                string inputY;
                cin >> inputY;
                
                // 转换输入为数字坐标
                if (input.length() == 1 && isalpha(input[0])) {
                    x = toupper(input[0]) - 'A' + 10;
                } else {
                    try {
                        x = stoi(input);
                    } catch (const exception& e) {
                        cout << "Invalid input! Please enter valid coordinates or commands." << endl;
                        continue;
                    }
                }
                
                if (inputY.length() == 1 && isalpha(inputY[0])) {
                    y = toupper(inputY[0]) - 'A' + 10;
                } else {
                    try {
                        y = stoi(inputY);
                    } catch (const exception& e) {
                        cout << "Invalid input! Please enter valid coordinates or commands." << endl;
                        continue;
                    }
                }
                
                // 在人类玩家落子前保存当前状态
                saveCurrentState();
            }
        } else {
            // AI玩家正常获取移动
            current->getMove(x, y, board);
        }
        
        // 在棋盘上放置棋子
        if (board.placeStone(x, y, current->getSymbol())) {
            lastX = x;
            lastY = y;
            
            // 检查游戏结束条件
            gameOver = checkGameEnd(x, y);
        } else if (currentPlayer == 1) {
            // 只有人类玩家会输入无效移动
            cout << "Invalid move! Please try again." << endl;
            continue;  // 不切换玩家，重新输入
        }
        
        // 如果游戏未结束且不返回主菜单，切换玩家
        if (!gameOver && !returnToMainMenu) {
            switchPlayer();
        }
    }
    
    // 如果是因为游戏结束（不是返回主菜单），显示结果并询问是否再玩
    if (gameOver && !returnToMainMenu) {
        displayStatus();
        if (board.isFull()) {
            cout << "Game Over! It's a draw!" << endl;
        } else {
            Player* winner = (currentPlayer == 1) ? player1.get() : player2.get();
            cout << "Game Over! " << winner->getName() << " wins!" << endl;
        }
        
        askPlayAgain();
    }
}

// 回退操作
void Game::undoMove() {
    if (!canUndo) {
        cout << "Cannot undo! No moves to undo." << endl;
        return;
    }
    
    cout << "Are you sure you want to undo the last move? (y/n): ";
    char choice;
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        // 恢复棋盘状态
        board = lastBoard;
        canUndo = false;
        
        // 确保当前玩家是人类玩家（因为只有人类玩家可以触发undo）
        currentPlayer = 1;
        
        cout << "Last move undone! It's your turn again." << endl;
    } else {
        cout << "Undo cancelled." << endl;
    }
}

void Game::switchPlayer() {
    currentPlayer = (currentPlayer == 1) ? 2 : 1;
}

bool Game::checkGameEnd(int lastX, int lastY) {
    Player* current = (currentPlayer == 1) ? player1.get() : player2.get();
    
    // 检查获胜
    if (board.checkWin(lastX, lastY, current->getSymbol())) {
        return true;
    }
    
    // 检查平局
    if (board.isFull()) {
        return true;
    }
    
    return false;
}

void Game::displayStatus() const {
    cout << "\n";
    board.display();
    cout << endl;
}

void Game::askPlayAgain() {
    cout << "\nWould you like to play again? (y/n): ";
    char choice;
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        // 重置游戏状态
        currentPlayer = 1;
        gameOver = false;
        returnToMainMenu = false;
        canUndo = false;
        startGame();
    } else {
        cout << "Thanks for playing!" << endl;
        returnToMainMenu = true;
    }
}

void Game::restartGame() {
    cout << "\nAre you sure you want to restart the game? (y/n): ";
    char choice;
    cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        cout << "Restarting game..." << endl;
        board.clear();
        currentPlayer = 1;
        gameOver = false;
        canUndo = false;
        // 注意：我们不重置难度，保持当前难度设置
    } else {
        cout << "Resuming current game..." << endl;
    }
}

bool Game::confirmExit() {
    cout << "\nWARNING: Unsaved game progress will be lost!" << endl;
    cout << "Are you sure you want to exit to main menu? (y/n): ";
    char choice;
    cin >> choice;
    
    return (choice == 'y' || choice == 'Y');
}

bool Game::saveGame(const string& filename) {
    string currentPlayerName = (currentPlayer == 1) ? player1->getName() : player2->getName();
    return SaveSystem::saveGame(filename, board, currentPlayerName, difficulty);
}

bool Game::loadGame(const string& filename) {
    string currentPlayerName;
    int loadedDifficulty;
    
    if (SaveSystem::loadGame(filename, board, currentPlayerName, loadedDifficulty)) {
        difficulty = loadedDifficulty;
        player2 = make_unique<AIPlayer>("Computer", 'O', difficulty);
        
        if (currentPlayerName == player1->getName()) {
            currentPlayer = 1;
        } else {
            currentPlayer = 2;
        }
        
        // 加载游戏后不能回退（因为没有之前的游戏状态）
        canUndo = false;
        
        cout << "Game loaded successfully!" << endl;
        gameLoop();
        return true;
    }
    
    cout << "Failed to load game!" << endl;
    return false;
}