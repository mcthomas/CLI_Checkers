/* ******************************************************
 * CS368 FINAL PROJECT: CLI_Checkers
 * Main File: checkers.cpp
 * Names: Matt Thomas, Aleks Lesiewicz, and Parker Breene
 *******************************************************/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Designates current state of the game
class Board {
    
    char black = '⬓';
    char red = '⬕';
    char blackKing = '⬒'
    char redKing = '⬔
    char space = '⧠'
    vector< vector<char> > board;
    board.resize(8, std::vector<int>(8, '⧠'));
    
    bool checkValid(int x, int y) {}
    
    bool checkOpen(int x, int y) {}
    
    void move(int x, int y, char symbol) {}
    
    bool checkEnd() {}
    
    bool checkTie() {}
    
    void updateBoard() {}
    
};

//Simple AI; randomly executes valid moves in turn
class AI {
    
};

//Handles gameplay loop / user input
int main(int argc, char**argv)
{
    return 0;
}
