/* ******************************************************
 * CS368 FINAL PROJECT: CLI_Checkers
 * Main File: checkers.cpp
 * Names: Matt Thomas, Aleks Lesiewicz, and Parker Breene
 *******************************************************/

#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

//Designates current state of the game
class Board {
public:
    
    string black = "⬓";
    string red = "⬕";
    string blackKing = "⬒";
    string redKing = "⬔";
    string space = "⧠";
    string board[8][8];
    string winner = "";
    
    //Verifies that the piece to move belongs to the player at turn
    bool validPiece(int x, int y, bool p) {
        if(p) {
            if(board[invert(y)][x] != black && (board[invert(y)][x] != blackKing)) {
                return false;
            }
        }
        else {
            if((board[invert(y)][x] != red) && (board[invert(y)][x] != redKing)) {
                return false;
            }
        }
        return true;
    }
    
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
        //Identifies the piece type at turn; updates to king if necessary
        string player = board[invert(thisY)][thisX];
        if((player == black) && (nextY == 7)) {
            player = blackKing;
        }
        else if((player == red) && (nextY == 0)) {
            player = redKing;
        }

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
    bool checkEnd() {
        bool b = false;
        bool r = false;
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 0; j++) {
                if(board[i][j] == black) {
                    b = true;
                }
                else if(board[i][j] == red) {
                    r = true;
                }
                if(b && r) {
                    return false;
                }
            }
        }
        if(!b) {
            winner = "Red";
        }
        else {
            winner = "Black";
        }
        return true;
    }
    
    string getWinner() {
        return winner;
    }
    
    //Array traversed to check for tie-game state
    bool checkTie() {
        return false;
    }
    
    void newBoard() {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 0; j++) {
                //If i s even
                if(i % 2 != 1) {
                    if(j % 2 == 1) {
                        if(i < 3) {
                            board[i][j] = red;
                        }
                        else if(i > 4) {
                            board[i][j] = black;
                        }
                    }
                }
                //If i is odd
                else {
                    if(j % 2 != 1) {
                        if(i < 3) {
                            board[i][j] = red;
                        }
                        else if(i > 4) {
                            board[i][j] = black;
                        }
                    }
                }
            }
        }
    }
    
    //Prints current state of the game board
    string updateBoard() {
        string tiles = "";
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                tiles += board[i][j];
            }
            tiles += "\n";
        }
        return tiles;
    }
   
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
    //Still skeleton code	
	int randThisX = -1;
	int randThisY = -1;
	int randNextX = -1;
	int randNextY = -1;
	    while(!validPiece(randThisX, randThisY, p) && !checkValid(randThisX, randThisY, randNextX, randNextY)) {
	        randThisX = rand() % 8 + 1;
	        randThisY = rand() % 8 + 1;
	        randNextX = randThisX + 1;
	        randNextY = randThisY + 1;
	            if(validPiece(randThisX, randThisY, p)) {
	                if(checkValid(randThisX, randThisY, randNextX, randNextY)){
	                    move(randThisX, randThisY, randNextX, randNextY);
	                } else {
	                    randNextX = randThisX - 1;
	                    randNextY = randThisY - 1;
	                        if(checkValid(randThisX, randThisY, randNextX, randNextY)){
	                            move(randThisX, randThisY, randNextX, randNextY);
	                        }
	                }
	        }
};

//Handles singleplayer / multiplayer gameplay loops
int main(int argc, char**argv)
{
    Board game;
    bool multi = false;
    string input = "";
    bool turnOne = true;
    printf("\nCheckers\n"
           , "=========================\n"
           , "Enter 1 for 1P or 2 for 2P: ");
    while(true) {
        getline (cin, input);
        if(input == "1") {
            break;
        }
        if(input == "2") {
            multi = true;
            break;
        }
        printf("\n" , "Invalid input. Enter 1 for 1P or 2 for 2P: ");
    }
    if(!multi) {
        printf("\n" , "Checkers for one: Player 1 - ⬓/⬒  CPU - ⬕/⬔" , "\n");
    }
    else {
        printf("\n" , "Checkers for two: Player 1 - ⬓/⬒  Player 2 - ⬕/⬔" , "\n");
    }
    int thisX = 0;
    int thisY = 0;
    int nextX = 0;
    int nextY = 0;
    input = "";
    game.newBoard();
    while(!game.checkEnd() && !game.checkTie()) {
        printf("\n" , game.updateBoard().c_str());
        while(!game.validPiece(thisX - 1, thisY - 1, turnOne)) {
            while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {
                printf("Enter the column # of piece to move: ");
                getline (cin, input);
                printf("\n");
            }
            thisX = stoi(input);
            input = "";
            while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {
                printf("Enter the row # of piece to move: ");
                getline (cin, input);
                printf("\n");
            }
            thisY = stoi(input);
            input = "";
            if(!game.validPiece(thisX - 1, thisY - 1, turnOne)) {
                printf("That tile is not occupied by one of your pieces.");
            }
        }

        input = "";
        while(!game.checkValid(thisX - 1, thisY - 1, nextX - 1, nextY - 1)) {
            while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {
                printf("Enter the column # of piece to move: ");
                getline (cin, input);
                printf("\n");
            }
            nextX = stoi(input);
            input = "";
            while((input != "1") && (input != "2") && (input != "3") && (input != "4") && (input != "5") && (input != "6") && (input != "7") && (input != "8")) {
                printf("Enter the row # of piece to move: ");
                getline (cin, input);
                printf("\n");
            }
            nextY = stoi(input);
            if(!game.checkValid(thisX - 1, thisY - 1, nextX - 1, nextY - 1)) {
                printf("That is not a valid move.");
            }
        }
        game.move(thisX - 1, thisY - 1, nextX - 1, nextY - 1);
        if(turnOne) {
            turnOne = false;
        }
        else {
            turnOne = true;
        }
        thisX = 0;
        thisY = 0;
        nextX = 0;
        nextY = 0;
        input = "";
    }
    if(game.checkEnd()) {
        printf("\n" , game.getWinner().c_str() , "wins!");
    }
    else {
        printf("\n" , "Tie game!");
    }
    
        
    return 0;
}
