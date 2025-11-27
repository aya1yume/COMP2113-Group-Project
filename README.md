# COMP2113-Group-Project
HKU COMP2113 2025-26 Sem1 Group Project

Five-in-a-Row Game

Team Members

Liu Yuxuan 3036445464

Chen Yu Wai 3036391900

# Application Description
Five-in-a-Row is a text-based console game implemented in C++ where players compete to connect five stones in a row horizontally, vertically, or diagonally. The game features human vs AI gameplay with multiple difficulty levels, save/load functionality, and an intuitive command-based interface.

# Features Implemented
## 1.Core Gameplay
Human vs AI Gameplay: Play against a computer opponent with three difficulty levels

15×15 Game Board: Standard Five-in-a-Row board size with hexadecimal coordinate system (0-9, A-E)

Win Condition Detection: Automatically detects when a player connects five stones

Draw Detection: Identifies when the board is full with no winner

## 2.Game Commands
Move Input: Enter coordinates (e.g., "7 7" or "A A") to place stones

Save Game: Type "save" during your turn to save progress

Load Game: Load previously saved games from the main menu

Undo Move: Type "undo" to revert your last move (human player only)

Restart Game: Type "restart" to start a new game

Exit to Menu: Type "exit" to return to the main menu

## 3.AI Difficulty Levels
Easy: Random moves

Medium: Basic defensive strategy with threat detection

Hard: Advanced strategy with evaluation functions and pattern recognition

# Coding Requirements Implementation
## 1.Generation of Random Events
Implementation: AI moves at easy difficulty use random number generation to select positions

Location: AIPlayer::easyMove() in AIPlayer.cpp

Purpose: Provides unpredictable gameplay at lower difficulty levels

## 2.Data Structures for Storing Data
Implementation: 2D vector (vector<vector<char>>) for the game board

Location: Board class in Board.h and Board.cpp

Purpose: Efficiently stores and manages the game state with O(1) access to any position

## 3.Dynamic Memory Management
Implementation: Smart pointers (unique_ptr) for player objects

Location: Game class in Game.h and Game.cpp

Purpose: Automatic memory management for player instances with clear ownership

## 4.File Input/Output
Implementation: Save/load system using file streams

Location: SaveSystem class in SaveSystem.h and SaveSystem.cpp

Purpose: Persistent game state storage with custom file format

## 5.Program Codes in Multiple Files
Implementation: Modular class structure with separate header and implementation files

Files:

main.cpp - Program entry point

Game.h/cpp - Game control logic

Board.h/cpp - Board state management

Player.h/cpp - Player base class

AIPlayer.h/cpp - AI player implementation

SaveSystem.h/cpp - Save/load functionality

Makefile - Build configuration

## 6.Multiple Difficulty Levels
Implementation: Three distinct AI strategies with increasing complexity

Location: AIPlayer class with easyMove(), mediumMove(), and hardMove() methods

Purpose: Provides scalable challenge for players of different skill levels

# Non-Standard Libraries
This project uses only standard C++ libraries (<iostream>, <vector>, <memory>, <fstream>, etc.) and requires no additional installations. All code is compatible with C++14 standard.

# Compilation and Execution Instructions
Prerequisites:

C++ compiler with C++14 support (g++ recommended)

Make utility (for using Makefile)

## Compilation:
bash

#Using Makefile (recommended)

make

#Or manual compilation

g++ -std=c++14 main.cpp Game.cpp Board.cpp Player.cpp AIPlayer.cpp SaveSystem.cpp -o five_in_a_row

## Execution
bash

./five_in_a_row

# Game Instructions
Select "Start New Game" from the main menu

Choose difficulty level (1-3)

During your turn, enter coordinates or commands:

Coordinates: "7 7" or "A A"

Save game: "save"

Undo move: "undo"

Restart: "restart"

Exit: "exit"

Connect five stones in a row to win!

# File Structure
text

five_in_a_row/

├── main.cpp

├── Game.h

├── Game.cpp

├── Board.h

├── Board.cpp

├── Player.h

├── Player.cpp

├── AIPlayer.h

├── AIPlayer.cpp

├── SaveSystem.h

├── SaveSystem.cpp

├── Makefile

└── README.md

# Video Demonstration
A 3-minute demonstration video is available at    showing:

Game startup and menu navigation

Gameplay at different difficulty levels

Save/load functionality

Undo feature

Win condition detection

*This project was developed for COMP2113 Course Project (2025-2026) 1st Semester*
