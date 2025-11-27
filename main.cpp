#include "Game.h"
#include "SaveSystem.h"
#include <iostream>
#include <string>
#include <limits>
#include <vector>

using namespace std;

int main() {
    Game game;
    int choice;
    
    while (true) {
        cout << "=== Five-in-a-Row Game ===" << endl;
        cout << "1. Start New Game" << endl;
        cout << "2. Load Game" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        
        cin >> choice;
        
        switch(choice) {
            case 1:
                game.startGame();
                break;
            case 2:
                {
                    // 显示可用的存档文件
                    vector<string> saveFiles = SaveSystem::listSaveFiles();
                    if (saveFiles.empty()) {
                        cout << "No save files found in current directory." << endl;
                        cout << "Save files should have .save extension." << endl;
                    } else {
                        cout << "Available save files:" << endl;
                        for (int i = 0; i < saveFiles.size(); i++) {
                            cout << "  " << (i + 1) << ". " << saveFiles[i] << endl;
                        }
                        
                        cout << "Enter the number of the save file to load (or 0 to cancel): ";
                        int fileChoice;
                        cin >> fileChoice;
                        
                        if (fileChoice == 0) {
                            cout << "Load cancelled." << endl;
                        } else if (fileChoice >= 1 && fileChoice <= saveFiles.size()) {
                            game.loadGame(saveFiles[fileChoice - 1]);
                        } else {
                            cout << "Invalid choice!" << endl;
                        }
                    }
                }
                break;
            case 3:
                cout << "Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        
        // 清空输入缓冲区
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;
    }
    
    return 0;
}