/* ******************************************************
 * CS368 FINAL PROJECT: CLI_Checkers
 * Main File: checkers.cpp
 * Names: Matt Thomas, Aleks Lesiewicz, and Parker Breene
 *******************************************************/

#include <iostream>
#include <string>

using namespace std;

//Designates current state of the game
class Board {
    
    string black = "⬓";
    string red = "⬕";
    string blackKing = "⬒";
    string redKing = "⬔";
    string space = "⧠";
    string board[8][8];
    
    //Verifies that the user-requested move is valid
    bool checkValid(int thisX, int thisY, int nextX, int nextY) {
        //Y-coordinates corrected for top-down row access of array
        int vThisY = invert(thisY);
        int vNextY = invert(nextY);
        //Identifies the current player / piece type and the opponent's
        string player = board[vThisY][thisX];
        string other = "";
        string otherK = "";
        if((player == black) || (player == blackKing)) {
            other = red;
            otherK = redKing;
        }
        else {
            other = black;
            otherK = blackKing;
        }
        //Checks if move is within the bounds of the board
        if(thisX < 0 || thisX > 7 || thisY < 0 || thisY > 7) {
            return false;
        }
        //Checks if move tile is unoccupied
        if(board[vNextY][nextX] != space) {
            return false;
        }
        //Varaibles to designate deviation from current position
        int absX = abs(thisX - nextX);
        int absY = abs(thisY - nextY);
        //Checks if move tile is diagonally located w/ respect to current position
        if((absX != absY) || absX == 0) {
            return false;
        }
        //Checks if move is valid in direction allotted by piece type
        if((player == black) && ((vThisY - vNextY) < 1)) {
            return false;
        }
        if((player == red) && ((vThisY - vNextY) > 1)) {
            return false;
        }
        //Permits single-tile movement
        if(absX == 1) {
            return true;
        }
        //Checks if move distance is a multiple of two tiles
        if(absX % 2 != 0) {
            return false;
        }
        //Variable to track increment span to move via jump(s)
        int inc = 2;
        //Four loops currated for 1 of 4 directions; checks if jump space(s) are open / there exist opponent's piece(s) to be jumped
        if(nextX < thisX && nextY < thisY) {
            while(inc <= absX) {
                if((((board[vThisY + inc - 1][thisX - inc + 1]) != other) || ((board[vThisY + inc - 1][thisX - inc + 1]) != otherK)) || (board[vThisY + inc][thisX - inc] != space)) {
                    return false;
                }
                inc += 2;
            }
        }
        else if(nextX > thisX && nextY < thisY) {
            while(inc <= absX) {
                if((((board[vThisY + inc - 1][thisX + inc - 1]) != other) || ((board[vThisY + inc - 1][thisX + inc - 1]) != otherK)) || (board[vThisY + inc][thisX + inc] != space)) {
                    return false;
                }
                inc += 2;
            }
        }
        else if(nextX < thisX && nextY > thisY) {
            while(inc <= absX) {
                if((((board[vThisY - inc + 1][thisX - inc + 1]) != other) || ((board[vThisY - inc + 1][thisX - inc + 1]) != otherK)) || (board[vThisY - inc][thisX - inc] != space)) {
                    return false;
                }
                inc += 2;
            }
        }
        else {
            while(inc <= absX) {
                if((((board[vThisY - inc + 1][thisX + inc - 1]) != other) || ((board[vThisY - inc + 1][thisX + inc - 1]) != otherK)) || (board[vThisY - inc][thisX + inc] != space)) {
                    return false;
                }
                inc += 2;
            }
        }
        return true;
    }
    
    //Updates char contents of array following checkValid returning true for given move
    void move(int thisX, int thisY, int nextX, int nextY) {
        //Identifies the player at turn
        string player = board[invert(thisY)][thisX];
        //Clears piece's initial tile
        board[invert(thisY)][thisX] = space;
        //Varaible to designate deviation from current position
        int absX = abs(thisX - nextX);
        //Updates both tiles for single-unit move
        if(absX == 1) {
            board[invert(nextY)][nextX] = player;
            return;
        }
        //Y-coordinates corrected for top-down row access of array
        int vThisY = invert(thisY);
        int vNextY = invert(nextY);
        //Variable to track increment span to move via jump(s)
        int inc = 2;

        //Four loops currated for 1 of 4 directions; updates symbols long movement's path
        if(nextX < thisX && nextY < thisY) {
            while(inc <= absX) {
                board[vThisY + inc - 1][thisX - inc + 1] = space;
                if(inc == absX) {
                    board[vThisY + inc][thisX - inc] = player;
                    return;
                }
                else {
                    board[vThisY + inc][thisX - inc] = space;
                }
                inc += 2;
            }
        }
        else if(nextX > thisX && nextY < thisY) {
            while(inc <= absX) {
                board[vThisY + inc - 1][thisX + inc - 1] = space;
                if(inc == absX) {
                    board[vThisY + inc][thisX + inc] = player;
                    return;
                }
                else {
                    board[vThisY + inc][thisX + inc] = space;
                }
                inc += 2;
            }
        }
        else if(nextX < thisX && nextY > thisY) {
            while(inc <= absX) {
                board[vThisY - inc + 1][thisX - inc + 1] = space;
                if(inc == absX) {
                    board[vThisY - inc][thisX - inc] = player;
                    return;
                }
                else {
                    board[vThisY - inc][thisX - inc] = space;
                }
                inc += 2;
            }
        }
        else {
            while(inc <= absX) {
                board[vThisY - inc + 1][thisX + inc - 1] = space;
                if(inc == absX) {
                    board[vThisY - inc][thisX + inc] = player;
                    return;
                }
                else {
                    board[vThisY - inc][thisX + inc] = space;
                }
                inc += 2;
            }
        }
        return;
    }
    
    //Checks player piece count for 0, indicating end-game state
    bool checkEnd() {/* TODO */}
    
    //Array traversed to check for tie-game state
    bool checkTie() {/* TODO */}
    
    void newBoard() {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 0; j++) {
                board[i][j] = space;
            }
        }
    }
    
    //Prints current state of the game board
    void updateBoard() {/* TODO */}
   
    //Inverts a y-coordinate for use in operations on the array
    int invert(int n) {
        if(n < 4) {
            return (7 - n);
        }
        if(n > 3) {
            return (0 + (7-n));
        }
    }
    
};

//Simple AI; randomly executes valid moves in turn
class AI {
    /* TODO */
};

//Handles singleplayer / multiplayer gameplay loops
int main(int argc, char**argv)
{
    /* TODO */
    return 0;
}
